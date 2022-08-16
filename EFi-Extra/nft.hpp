#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/print.hpp>
#include <eosio/system.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("nftcontrak")]] nftcontrak:public eosio::contract 
{
    private:
    const symbol eos_symbol;

    struct [[eosio::table]] buyers_total
    {
        name        owner_account;
        uint16_t    total_hub;
        uint16_t    total_dop;
        uint16_t    total_dmd;
        uint64_t primary_key()const { return owner_account.value; }
    };
    typedef eosio::multi_index< "countable"_n, buyers_total > countable;

    struct [[eosio::table]] nft_mints
    {
        uint16_t  mint_order;
        name      owner_account;
        uint16_t  amount_hub;
        uint16_t  amount_dop;
        uint16_t  amount_dmd;
        uint16_t primary_key()const { return mint_order; }
    };
    typedef eosio::multi_index< "mintingtable"_n, nft_mints > mintingtable;

    struct [[eosio::table]] total_minted 
    {
        name        key;
        uint16_t    hub_total_minted;
        uint16_t    dop_total_minted;
        uint16_t    dmd_total_minted;
        uint64_t primary_key()const { return key.value; } 
    };
    typedef eosio::multi_index< "totaltable"_n, total_minted > totaltable;

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

    struct ATTRIBUTE_MAP
    {

    };

    //void registernft(const name& owner_account, const name& to, const asset& swap_quantity_hub, std::string memo);
    void nftburn(const name asset_owner, uint64_t asset_id, const name collection_name, const name schema_name, int32_t template_id, 
    vector <asset> backed_tokens, ATTRIBUTE_MAP old_immutable_data, ATTRIBUTE_MAP old_mutable_data, const name asset_ram_payer);
    //void nfttransfer(const name collection_name, const name from, const name to, const vector<uint64_t> asset_ids, const std::string memo);

    [[eosio::action]]
    void set();

    nftcontrak(name receiver, name code, datastream<const char *> ds):contract(receiver, code, ds), eos_symbol("EOS", 4){}
};
