#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/print.hpp>
#include <eosio/system.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("burncontrak")]] burncontrak:public eosio::contract 
{
    private:
    const symbol hub_symbol;
    const symbol dop_symbol;
    const symbol dmd_symbol;
    const symbol xhub_symbol;
    const symbol xdop_symbol;
    const symbol xdmd_symbol;

    struct [[eosio::table]] burnerdata 
    {
        name     key;
        bool xseason_burn_open;
        bool xseason_stake_open;
        bool xseason_rewards_open;

        uint64_t primary_key()const { return key.value; } 
    };
    
    typedef eosio::multi_index< "totaltable"_n, burnerdata > totaltable;

    void inline_burnhub(eosio::name owner_account, eosio::asset quantity, std::string memo) const
    {
        struct burn
        {
            eosio::name owner_account;
            eosio::asset quantity;
            std::string memo;
        };

        eosio::action burn_action = eosio::action(
            eosio::permission_level(get_self(), "active"_n),
            eosio::name("hub.efi"), // name of the contract
            eosio::name("burn"),
            burn{owner_account, quantity, memo});
            burn_action.send();
    }

    void inline_burndop(eosio::name owner_account, eosio::asset quantity, std::string memo) const
    {
        struct burn
        {
            eosio::name owner_account;
            eosio::asset quantity;
            std::string memo;
        };

        eosio::action burn_action = eosio::action(
            eosio::permission_level(get_self(), "active"_n),
            eosio::name("dop.efi"), // name of the contract
            eosio::name("burn"),
            burn{owner_account, quantity, memo});
            burn_action.send();
    }

    void inline_burndmd(eosio::name owner_account, eosio::asset quantity, std::string memo) const
    {
        struct burn
        {
            eosio::name owner_account;
            eosio::asset quantity;
            std::string memo;
        };

        eosio::action burn_action = eosio::action(
            eosio::permission_level(get_self(), "active"_n),
            eosio::name("dmd.efi"), // name of the contract
            eosio::name("burn"),
            burn{owner_account, quantity, memo});
            burn_action.send();
    }

    void inline_transferxtoken(eosio::name from, eosio::name to, eosio::asset quantity, std::string memo) const
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
            eosio::name("xtoken.efi"), // name of the contract
            eosio::name("transfer"),
            transfer{from, to, quantity, memo});
            transfer_action.send();
    }

    public:
    using contract::contract;

    void burndop(const name& owner_account, const name& to, const asset& burn_quantity_dop, std::string memo);
    void burnhub(const name& owner_account, const name& to, const asset& burn_quantity_hub, std::string memo);
    void burndmd(const name& owner_account, const name& to, const asset& burn_quantity_dmd, std::string memo);

    [[eosio::action]]
    void set(bool xseason_open);

    burncontrak(name receiver, name code, datastream<const char *> ds):contract(receiver, code, ds), 
                                     hub_symbol("HUB", 4), dop_symbol("DOP", 4), dmd_symbol("DMD", 4),
                                     xhub_symbol("XHUB", 4), xdop_symbol("XDOP", 4), xdmd_symbol("XDMD", 4){}
};
