#include <dmdao.hpp>

/* The DMD Dao Smart Contract */

/* This will be used to vote on the final inflation figures before we null the keys to DMD */
/* On notify, on receive DMD, activate voting function */
/* User sends a minimum of 1 DMD to vote for any of the 5 proposals */
/* A user can vote as many times as he wants, for as many proposals as he wants */
/* Send min. 1 DMD to the dao vote contract, with memo "1" to "5" and the vote will be registered in the according proposal */
/* We will also have a total_votes counter, and we will have a proposal_1_total, to proposal_5_total votes */

/* We will also have another table, with each user, and their amount of DMD that they used to vote, doesn't matter which DMD has voted for which pool, just the total */

/* The percentage is simply proposal_votes *100 / total_votes */

/* We will have an end_vote function which will refund everyone their DMD and set the final percentage numbers for each of the five proposals */

void dmdao::set()
{
    require_auth(get_self());
    totaltable total_votes(get_self(), "totals"_n.value);

    auto total_it = total_votes.find("totals"_n.value);
    check(total_it == total_votes.end(), "totals table has already been set.");

    total_votes.emplace(get_self(), [&](auto& row) 
    {   /* Just set everything to ZERO, almost. Not really everything. */
        row.key                   = "totals"_n;
        row.total_votes           = 0;
        row.proposal1_total_votes = 0;
        row.proposal2_total_votes = 0;
        row.proposal3_total_votes = 0;
        row.proposal4_total_votes = 0;
        row.proposal5_total_votes = 0;
        row.locked                = 1;
    });
}

void dmdao::setlocked(bool locked)
{ /* This function lets us set the 'locked' variable true or false for the 31 day staking. */
    require_auth(get_self());

    totaltable total_votes(get_self(), "totals"_n.value); 
    auto total_it = total_votes.find("totals"_n.value);
    check(total_it != total_votes.end(),"error: need to init totals table first.") ;
    total_votes.modify(total_it, get_self(), [&]( auto& row ) 
    {
        row.locked = locked;
    });
}

[[eosio::on_notify("dmd.efi::transfer")]]
void dmdao::vote(const name& owner_account, const name& to, const asset& vote_quantity_dmd, std::string memo)
{
    if (to != get_self() || owner_account == get_self())
    {
        print("error: these are not the droids you are looking for.");
        return;
    }

    /* Check for valid memo */
    bool validmemo = false;
    if ((memo == "1") || (memo == "2") || (memo == "3") || (memo == "4") || (memo == "5"))
    {
        validmemo = true;
    }
    check(validmemo  == true, "error: memo must be between 1 and 5.");

    check(vote_quantity_dmd.amount >= 10000, "error: 1 DMD is the minimum required amount to cast a vote.");
    check(vote_quantity_dmd.symbol == dmd_symbol, "error: these are not the droids you are looking for.");

    totaltable total_votes(get_self(), "totals"_n.value);
    auto total_it = total_votes.find("totals"_n.value);
    check(total_it != total_votes.end(), "error: totals table is not initiated."); 
    check(total_it->locked == false, "error: the voting contract is locked.");

    /* Set temp variables accordingly, based on memo. */
    uint64_t proposal1_addition = 0; 
    uint64_t proposal2_addition = 0; 
    uint64_t proposal3_addition = 0; 
    uint64_t proposal4_addition = 0; 
    uint64_t proposal5_addition = 0;

    if (memo == "1") proposal1_addition = vote_quantity_dmd.amount;
    if (memo == "2") proposal2_addition = vote_quantity_dmd.amount;
    if (memo == "3") proposal3_addition = vote_quantity_dmd.amount;
    if (memo == "4") proposal4_addition = vote_quantity_dmd.amount;
    if (memo == "5") proposal5_addition = vote_quantity_dmd.amount;

    /* Update the player's total_votes and the individual proposal votes */
    total_votes.modify(total_it, get_self(), [&](auto& row) 
    {
        row.total_votes += vote_quantity_dmd.amount;

        row.proposal1_total_votes += proposal1_addition;
        row.proposal2_total_votes += proposal2_addition;
        row.proposal3_total_votes += proposal3_addition;
        row.proposal4_total_votes += proposal4_addition;
        row.proposal5_total_votes += proposal5_addition;
    });

    votetable individual_votes(get_self(), get_self().value);

    auto vote_it = individual_votes.find(owner_account.value);
    if(vote_it == individual_votes.end())
    { /* First time depositing anything into staking */
        individual_votes.emplace(get_self(), [&](auto& row)
        {
            row.owner_account = owner_account;

            row.total_player_votes += vote_quantity_dmd.amount;

            row.proposal1_player_votes += proposal1_addition;
            row.proposal2_player_votes += proposal2_addition;
            row.proposal3_player_votes += proposal3_addition;
            row.proposal4_player_votes += proposal4_addition;
            row.proposal5_player_votes += proposal5_addition;
        });
    }
    else
    { /* He already has an entry in the inidivual_votes table. We just modify it now. */
        individual_votes.modify(vote_it, get_self(), [&](auto& row) 
        {
            row.total_player_votes += vote_quantity_dmd.amount;

            row.proposal1_player_votes += proposal1_addition;
            row.proposal2_player_votes += proposal2_addition;
            row.proposal3_player_votes += proposal3_addition;
            row.proposal4_player_votes += proposal4_addition;
            row.proposal5_player_votes += proposal5_addition;
        });
    }
}

void dmdao::endvote()
{   /* Call this to end the voting and refund all the users their DMD */
    require_auth(get_self());

    totaltable total_votes(get_self(), "totals"_n.value);
    auto total_it = total_votes.find("totals"_n.value);
    check(total_it != total_votes.end(), "error: totals table is not initiated."); 
    check(total_it->locked == true, "error: the dmdao contract needs to be locked first!");

    votetable individual_votes(get_self(), get_self().value);
    auto begin_itr = individual_votes.begin();
    auto end_itr = individual_votes.end();

    while (begin_itr != end_itr)
    {
        eosio::print_f("Returning DMD to: [%]\n",begin_itr->owner_account);

        asset dmd_refund_amount;
        dmd_refund_amount.symbol = dmd_symbol;
        dmd_refund_amount.amount = begin_itr->total_player_votes/10000; /* We must divide by 10k for coins with precision 4 */

        /* Send rewards */
        dmdao::inline_transferdmd(get_self(), begin_itr->owner_account, dmd_refund_amount, "Refunding DMD. Thank you for voting in the DMD DAO!");

        individual_votes.modify(begin_itr, get_self(), [&]( auto& row ) 
        { /* Reset his total votes to 0, but leave his individual pool votes intact. */
            row.total_player_votes = 0;
        });
        ++ begin_itr;
    } /* Once we end the vote, we will not start a new one for this cause. */
}
