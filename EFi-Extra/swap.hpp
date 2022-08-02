#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/print.hpp>
#include <eosio/system.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("swapcontrak")]] swapcontrak:public eosio::contract 
{
    private:
    const symbol hub_symbol;
    const symbol dop_symbol;
    const symbol dmd_symbol;

    struct [[eosio::table]] swapped_accounts
    {
        name     owner_account;
        asset    hub_swapped_amount;
        asset    hub_received_amount;

        asset    dop_swapped_amount;
        asset    dop_received_amount;

        asset    dmd_swapped_amount;
        asset    dmd_received_amount;
        uint64_t primary_key()const { return owner_account.value; }
    };
    typedef eosio::multi_index< "swaptable"_n, swapped_accounts > swaptable;

    struct [[eosio::table]] total_swapped 
    {
        name     key;
        asset    hub_total_swapped;
        asset    dop_total_swapped;
        asset    dmd_total_swapped;
        uint8_t  bonus;
        bool     locked;
        uint64_t primary_key()const { return key.value; } 
    };
    
    typedef eosio::multi_index< "totaltable"_n, total_swapped > totaltable;


    void inline_transferhub(eosio::name from, eosio::name to, eosio::asset quantity, std::string memo) const
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

    void registerswaphub(const name& owner_account, const name& to, const asset& swap_quantity_hub, std::string memo);
    void registerswapdop(const name& owner_account, const name& to, const asset& swap_quantity_dop, std::string memo);
    void registerswapdmd(const name& owner_account, const name& to, const asset& swap_quantity_dmd, std::string memo);

    [[eosio::action]]
    void set(const asset& initial_hub, const asset& initial_dop, const asset& initial_dmd, uint8_t initial_bonus);
    [[eosio::action]]
    void setbonus(uint8_t bonus);
    [[eosio::action]]
    void setlocked(bool locked);

    swapcontrak(name receiver, name code, datastream<const char *> ds):contract(receiver, code, ds), 
                                     hub_symbol("HUB", 4), dop_symbol("DOP", 8), dmd_symbol("DMD", 10){}
};
