/* This is the EFi NFT presale minting contract */
/* This is the simplest of the EFi contracts thus far */
// Users can send EOS to the NFT contract.
// The contract must check that not all the NFTs have been reserved (sold out)
// After that the contract must check how much EOS has been sent
// If they send too little EOS, refund them (or keep the EOS and ask to send the rest)
// You could have a scenario where you don't refund anyone at all. You just take EOS and reserve any number of NFTs.
// If by any chance we sell more than 100, then so be it, we sell out, we raise the number of total NFTs, I mean, they're already bought aren't they?
// So it just accepts EOS and logs each user in a table and their number of each of the DMD, DOP or HUB NFTs.
// Maybe we only need to have one table.
// We need a totals table and a buyers table
// Totals table just holds the number of NFTs sold for each of the three coins
// The buyers table will contain information about the order in which the NFTs were bought, and who bought them.
// The buyers table will log each purchase:
// order_nr(key) : account_name : amount_dmd : amount_dop : amount_hub
// So a person can have multiple buys, and multiple NFTs bought in each of these buys, but a single transfer and subsequent purchase will only ever be for a single Coin (DMD or HUB or DOP).
// A user has to do three separate transfers to get all three NFTs
// Users will send their EOS to nft.efi with memo "dmd" or "hub" or "dop"
#include <nft.hpp>
/*
This is the EFi V2 NFT Contract.
*/

void nftcontrak::set()
{  // We will initiate the three tables here, and then that's basically all we gotta do.
    require_auth(get_self());

    totaltable total_minted(get_self(), "totals"_n.value); 
    auto total_it = total_minted.find("totals"_n.value);
    if(total_it == total_minted.end())
    {   // Create the "totals" row in the totals table and init it with zero for each column.
        total_minted.emplace(get_self(), [&](auto& row) 
        {
            row.key = "totals"_n;
            row.hub_total_minted = 0;
            row.dop_total_minted = 0;
            row.dmd_total_minted = 0;
        });
    }
    
    countable buyers_total(get_self(), get_self().value);
    auto buyers_it = buyers_total.find(get_self().value);
    if(buyers_it == buyers_total.end())
    {
        buyers_total.emplace(get_self(), [&](auto& row) 
        {
            row.owner_account = get_self();
            row.total_hub     = 0;
            row.total_dop     = 0;
            row.total_dmd     = 0;
        });
    }

    mintingtable nft_mints(get_self(), get_self().value);
    auto mints_it = nft_mints.find(get_self().value);
    if(mints_it == nft_mints.end())
    {
        nft_mints.emplace(get_self(), [&](auto& row) 
        {
            row.mint_order    = 0;
            row.owner_account = get_self();
            row.amount_hub    = 0;
            row.amount_dop    = 0;
            row.amount_dmd    = 0;
        });
    }
}
/*

[[eosio::on_notify("atomicassets::logtransfer")]]
void nftcontrak::nfttransfer(const name collection_name, const name from, const name to, const vector<uint64_t> asset_ids, const std::string memo)
{
    if (from != "nft.efi"_n)
    {
        check(to == "nft.efi"_n,"can only transfer NFTs to the nft.efi at the moment");
    }
}

*/

// Handles NFT burning to reward users with tokens.
[[eosio::on_notify("atomicassets::logburnasset")]]
void nftcontrak::nftburn(const name asset_owner, uint64_t asset_id, const name collection_name, const name schema_name, int32_t template_id, 
    vector <asset> backed_tokens, ATTRIBUTE_MAP old_immutable_data, ATTRIBUTE_MAP old_mutable_data, const name asset_ram_payer)
{
    //check(false,"NFT Burning is currently suspended while updating the contract. Please come back later.");
    asset dop_refund = asset(3500000, symbol("DOP", 4));   //  350 DOP
    asset dmd_refund = asset(27000,   symbol("DMD", 4));  //   2.7 DMD
    asset hub_refund = asset(2000000, symbol("HUB", 4)); //    200 HUB

    /* DOP */
    if ((collection_name == "nft.efi"_n) && (schema_name == "golden.dop"_n))
    {
        print("We have detected a golden.dop burn!");
        nftcontrak::inline_transferdop(get_self(), asset_owner, dop_refund, "The market gods are please with this sacrifice!");
    }

    /* DMD */
    if ((collection_name == "nft.efi"_n) && (schema_name == "golden.dmd"_n))
    {
        print("We have detected a golden.dmd burn!");
        nftcontrak::inline_transferdmd(get_self(), asset_owner, dmd_refund, "The market gods are please with this sacrifice!");
    }

    /* HUB */
    if ((collection_name == "nft.efi"_n) && (schema_name == "golden.hub"_n))
    {
        print("We have detected a golden.hub burn!");
        nftcontrak::inline_transferhub(get_self(), asset_owner, hub_refund, "The market gods are please with this sacrifice!");
    }
}
/*
[[eosio::on_notify("eosio.token::transfer")]]
void nftcontrak::registernft(const name& owner_account, const name& to, const asset& amount_eos_sent, std::string memo)
{
    if (owner_account != get_self())
    {
        if (owner_account == "efi"_n || owner_account == "funds.efi"_n)
        {
            return;
        }
        //check(false,"NFT Minting is currently suspended while updating the contract. Please come back later.");
    }
    check(false,"NFT minting is over. Please check AtomicHUB to purchase the NFTs.");
    check(amount_eos_sent.symbol == eos_symbol, "error: these are not the droids you are looking for.");
    // Checks how much EOS has been sent and what the memo is.
    // Registers the user in the buyers and mints tables. Updates the total table.
    if (to != get_self() || owner_account == get_self() || owner_account == "efi"_n)
    {
        print("error: these are not the droids you are looking for.");
        return;
    }

    uint64_t nft_price = 270000; // 27 EOS
    bool incorrectmemo = false;
    if ( (memo != "hub") && (memo != "dop") && (memo != "dmd") )
    {
        incorrectmemo = true;
    }
    check(!incorrectmemo,"error: you must specify the correct memo: `hub` or `dop` or `dmd`");
    check(amount_eos_sent.amount >= nft_price, "error: you must send at least 27 EOS to mint a Golden NFT.");

    totaltable total_minted(get_self(), "totals"_n.value);
    auto total_it = total_minted.find("totals"_n.value);
    check(total_it != total_minted.end(), "error: totals table is not initiated."); 
    // Here we check to see if the user wants to buy extra NFTs, over the 100 that is allowed.
    bool overflow = false;
    uint16_t limit = 137;

    // Do a check to see if we've already reached over the limit.
    // The only way for a user to buy over the 137 limit, is if the counter is at 135 or something, and they buy more than 2.
    // In that case, we will just manually refund the users.
    if ( (memo == "hub" && total_it->hub_total_minted >=limit) || 
           (memo == "dop" && total_it->dop_total_minted >=limit) || 
             (memo == "dmd" && total_it->dmd_total_minted >=limit) )
    {
        overflow = true;
    }

    check(!overflow, "error: the maximum number for this NFT has been reached (137 have been minted)");
    // Let's calculate how many NFTs the user has bought at this point, by looking at how much EOS they've sent.
    eosio::print_f("User has sent: [%] EOS \n",amount_eos_sent.amount);
    eosio::print_f("NFT Price: [%]\n",nft_price);

    uint16_t minted_nfts = amount_eos_sent.amount/nft_price;
    eosio::print_f("Number of minted NFTs: [%]\n",minted_nfts);
    uint16_t hub_minted = 0;
    uint16_t dop_minted = 0;
    uint16_t dmd_minted = 0;

    if (memo == "hub")
        hub_minted = minted_nfts;
    else if (memo == "dop")
        dop_minted = minted_nfts;
    else
        dmd_minted = minted_nfts;

    // Update the three tables:
    total_minted.modify(total_it, get_self(), [&](auto& row) 
    {
        row.hub_total_minted += hub_minted;
        row.dop_total_minted += dop_minted;
        row.dmd_total_minted += dmd_minted;
    });
    
    countable buyers_total(get_self(), get_self().value);
    auto buyers_it = buyers_total.find(owner_account.value);
    if(buyers_it == buyers_total.end())
    {
        buyers_total.emplace(get_self(), [&](auto& row)
        {
            row.owner_account = owner_account;
            row.total_hub     = hub_minted;
            row.total_dop     = dop_minted;
            row.total_dmd     = dmd_minted;
        });
    }
    else
    {
        buyers_total.modify(buyers_it, get_self(), [&](auto& row)
        {
            row.total_hub += hub_minted;
            row.total_dop += dop_minted;
            row.total_dmd += dmd_minted;
        });
    }

    mintingtable nft_mints(get_self(), get_self().value);
    auto begin_itr = nft_mints.begin();
    auto end_itr = nft_mints.end();
    uint16_t counter = 0;

    while (begin_itr != end_itr)
    {
        counter++;
        begin_itr++;
    }
    eosio::print_f("nft_mints table row count is at: [%]\n",counter);
    nft_mints.emplace(get_self(), [&](auto& row) 
    {
        row.mint_order    = counter;
        row.owner_account = owner_account;
        row.amount_hub    = hub_minted;
        row.amount_dop    = dop_minted;
        row.amount_dmd    = dmd_minted;
    });

    uint32_t refund_amount = amount_eos_sent.amount - minted_nfts*nft_price;

    eosio::print_f("Refund for user: [%]\n",refund_amount);
    if (refund_amount > 0)
    {
        asset refund_asset = amount_eos_sent;
        refund_asset.amount = refund_amount;
        nftcontrak::inline_transfereos(get_self(), owner_account, refund_asset, "Sending refund from the Golden NFT minting.");
    }
}
*/
