/* DMD DAO HPP */

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/print.hpp>
#include <eosio/system.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("dmdao")]] dmdao:public eosio::contract 
{
    private:
    const symbol dmd_symbol;

    struct [[eosio::table]] total_votes
    {
        name     key;

        bool     locked;

        uint64_t total_votes;

        uint64_t proposal1_total_votes;
        uint64_t proposal2_total_votes;
        uint64_t proposal3_total_votes;
        uint64_t proposal4_total_votes;
        uint64_t proposal5_total_votes;
        
        uint64_t primary_key()const { return key.value; } 
    };
    typedef eosio::multi_index< "totaltable"_n, total_votes > totaltable;


    struct [[eosio::table]] individual_votes
    {
        name     owner_account;

        uint64_t total_player_votes;

        uint64_t proposal1_player_votes;
        uint64_t proposal2_player_votes;
        uint64_t proposal3_player_votes;
        uint64_t proposal4_player_votes;
        uint64_t proposal5_player_votes;


        uint64_t primary_key()const { return owner_account.value; }
    };
    typedef eosio::multi_index< "votetable"_n, individual_votes > votetable;

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

    void vote(const name& owner_account, const name& to, const asset& vote_quantity_dmd, std::string memo);

    [[eosio::action]]
    void setlocked(bool locked);

    [[eosio::action]]
    void set();

    [[eosio::action]]
    void endvote();

    dmdao(name receiver, name code, datastream<const char *> ds):contract(receiver, code, ds), dmd_symbol("DMD", 4){}
};
