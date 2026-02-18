#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "raylib.h"

using namespace std;

template<typename Asst>

class AssetManager {
    private:
        map<string, vector<Asst>> AssetLookup;

        Asst LoadAssetUsingFile(const string& filepath) {
            return LoadTexture(filepath.c_str());
        }

        void UnloadAsset(const Asst& asset) {
            UnloadTexture(asset);
        }
    public:

        void LoadAsset(const string& name, const string& filepath) {
            Asst Asset = LoadAssetUsingFile(filepath);// LoadAsset() deprecates LoadTexture(), hence
            EnlistAsset(Asset, name); // this should always be used for future work in place of LoadTexture()
        }

        void UnloadAsset(const string& name, int index = 0) {
            auto AssetEntry = AssetLookup.find(name);

            if (AssetEntry != AssetLookup.end() && index < AssetEntry->second.size()) {
                UnloadAsset(AssetEntry->second[index]); // This is a raylib overload
                AssetEntry->second.erase(AssetEntry->second.begin() + index); // Erase the index in the map under the name

                if (AssetEntry->second.empty()) {
                    AssetLookup.erase(AssetEntry);
                }
            }
        }

        void EnlistAsset(const Asst& asset, const string& name) {
            AssetLookup[name].push_back(asset); // Add to the vector Asst
        }

        vector<Asst> GetAssetsInName(const string& name) {
            auto AssetEntry = AssetLookup.find(name);

            if (AssetEntry == AssetLookup.end()) { return {}; }

            return AssetEntry->second;
        }

        bool HasAsset(const string& name) {
            return AssetLookup.find(name) != AssetLookup.end();
        }

        int GetAssetInstCount(const string& name) {
            auto AssetEntry = AssetLookup.find(name);

            if (AssetEntry == AssetLookup.end()) { return 0; }

            return AssetEntry->second.size();
        }

        void DeleteAssetList() {
            AssetLookup.clear();
        }

        ~AssetManager() {
            for (auto& [Name, Assets] : AssetLookup) {
                for (auto& Asset : Assets) {
                    UnloadAsset(Asset); // Auto cleanup so data is not leftover (does not mean memory leak here)
                }
            }
        }
};

#endif