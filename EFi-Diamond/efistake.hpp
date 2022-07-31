#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/print.hpp>
#include <eosio/system.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("stake")]] stake:public eosio::contract 
{
    private:
    const symbol hub_symbol;
    const symbol dop_symbol;
    const symbol dmd_symbol;

    struct [[eosio::table]] staked_accounts
    {
        name     owner_account;
        
        asset    hub_staked_amount;
        asset    hub_claimed_amount;
        asset    hub_unclaimed_amount;

        asset    dop_staked_amount;
        asset    dop_claimed_amount;
        asset    dop_unclaimed_amount;

        asset    dmd_staked_amount;
        asset    dmd_claimed_amount;
        asset    dmd_unclaimed_amount;

        uint64_t primary_key()const { return owner_account.value; }
    };
    typedef eosio::multi_index< "staketable"_n, staked_accounts > staketable;

    struct [[eosio::table]] total_staked 
    {
        name     key;
        asset    hub_total_staked;
        asset    dop_total_staked;
        asset    dmd_total_staked;
        bool     locked;
        uint32_t last_reward_time;
        uint32_t hub_issue_frequency;
        uint32_t dop_issue_frequency;
        uint32_t dmd_issue_frequency;
        uint64_t primary_key()const { return key.value; } 
    };
    
    typedef eosio::multi_index< "totaltable"_n, total_staked > totaltable;


    void inline_transferhub(eosio::name from, eosio::name to, eosio::asset quantity, std::string memo) const
    { /* We'll use this when users claim() their rewards as well as at the end of the staking period when users withdraw their coins */
        struct transfer
        {
            eosio::name from;
            eosio::name to;
            eosio::asset quantity;
            std::string memo;
        };

        eosio::action transfer_action = eosio::action(
            eosio::permission_level(get_self(), "active"_n),
            eosio::name("hub.efi"), // name of the contract
            eosio::name("transfer"),
            transfer{from, to, quantity, memo});
            transfer_action.send();
    }

    void inline_transferdop(eosio::name from, eosio::name to, eosio::asset quantity, std::string memo) const
    {
        struct transfer
        {
            eosio::name from;
            eosio::name to;
            eosio::asset quantity;
            std::string memo;
        };

        eosio::action transfer_action = eosio::action(
            eosio::permission_level(get_self(), "active"_n),
            eosio::name("dop.efi"), // name of the contract
            eosio::name("transfer"),
            transfer{from, to, quantity, memo});
            transfer_action.send();
    }

    void inline_transferdmd(eosio::name from, eosio::name to, eosio::asset quantity, std::string memo) const
    {
        struct transfer
        {
            eosio::name from;
            eosio::name to;
            eosio::asset quantity;
            std::string memo;
        };

        eosio::action transfer_action = eosio::action(
            eosio::permission_level(get_self(), "active"_n),
            eosio::name("dmd.efi"), // name of the contract
            eosio::name("transfer"),
            transfer{from, to, quantity, memo});
            transfer_action.send();
    }

    public:
    using contract::contract;

    void stakehub(const name& owner_account, const name& to, const asset& swap_quantity_hub, std::string memo);
    void stakedop(const name& owner_account, const name& to, const asset& swap_quantity_dop, std::string memo);
    void stakedmd(const name& owner_account, const name& to, const asset& swap_quantity_dmd, std::string memo);

    [[eosio::action]]
    void set(const asset& total_staked_hub, const asset& total_staked_dop, const asset& total_staked_dmd,
                   uint32_t hub_issue_frequency, uint32_t dop_issue_frequency, uint32_t dmd_issue_frequency);
    [[eosio::action]]
    void setlocked(bool locked);
    [[eosio::action]]
    void claimhub(const name& owner_account);
    [[eosio::action]]
    void claimdop(const name& owner_account);
    [[eosio::action]]
    void claimdmd(const name& owner_account);
    [[eosio::action]]
    void withdraw(const name& owner_account);
    [[eosio::action]]
    void issue();

    stake(name receiver, name code, datastream<const char *> ds):contract(receiver, code, ds), 
                                     hub_symbol("HUB", 4), dop_symbol("DOP", 4), dmd_symbol("DMD", 4){}
};
