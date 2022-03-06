#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/print.hpp>
#include <eosio/system.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("nftgiveaway")]] nftgiveaway:public eosio::contract 
{
    private:
    const symbol eos_symbol;

    struct [[eosio::table]] winnerstb
    {
        name owner_account;
        vector<uint16_t> winners;
        uint64_t    primary_key()const { return owner_account.value; }
    };
    typedef eosio::multi_index< "winnertable"_n, winnerstb > winnertable;

    struct [[eosio::table]] players
    {
        name        owner_account;
        uint32_t    seed;
        uint64_t    primary_key()const { return owner_account.value; }
    };
    typedef eosio::multi_index< "playertable"_n, players > playertable;

    struct [[eosio::table]] globals 
    {
        name        key;
        bool        locked;
        uint64_t primary_key()const { return key.value; } 
    };
    typedef eosio::multi_index< "totaltable"_n, globals > totaltable;

    void inline_transfereos(eosio::name from, eosio::name to, eosio::asset quantity, std::string memo) const
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
            eosio::name("eosio.token"), // name of the contract
            eosio::name("transfer"),
            transfer{from, to, quantity, memo});
            transfer_action.send();
    }

    public:
    using contract::contract;

    void registergiveaway(const name& owner_account, const name& to, const asset& amount_eos_sent, std::string memo);

    [[eosio::action]]
    void cleartables();

    [[eosio::action]]
    void populate(const name& owner_account);

    [[eosio::action]]
    void setlocked(bool locked);

    [[eosio::action]]
    void getwinners();

    [[eosio::action]]
    void refund();

    nftgiveaway(name receiver, name code, datastream<const char *> ds):contract(receiver, code, ds), eos_symbol("EOS", 4){}
};
