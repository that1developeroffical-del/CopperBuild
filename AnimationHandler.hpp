#ifndef ANIMATIONHANDLER_HPP
#define ANIMATIONHANDLER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include "AssetManager.hpp"

using namespace std;

template<typename Spr, typename Inst>

class AnimationHandler {
    private:
        map<string, vector<string>> AnimationFrames;
        map<string, float> FrameDurations;

        AssetManager<Spr>& Assets;

        struct AnimationTrack {
            string NowAnimation;
            int NowFrame;
            chrono::steady_clock::time_point LastFrameSecond;
        };

        map<Inst&, AnimationTrack> AnimationTracks;
    public:
        AnimationHandler(AssetManager<Spr>& manager) : Assets(manager) {}

        void LoadAnimation(const string& name, const vector<string>& frameNames, float duration) {
            AnimationFrames[name] = frameNames;
            FrameDurations[name] = duration;

            int Count = 0;

            for (auto& FrameName : frameNames) {
                if (!Assets.HasAsset(FrameName)) {
                    cout << "Warning : Frame " << FrameName << " not loaded" << endl;
                    ++Count;
                }
            }

            if (Count > 0) { AnimationFrames.erase(name); FrameDurations.erase(name); return; }
        }

        void PlayTrack(const Inst& object, const string& animName) {
            if (AnimationFrames.find(animName) == AnimationFrames.end()) {
                cout << "Error: Animation '" << animName << "' not found" << endl;
                return;
            }

            AnimationTrack State;
            State.NowAnimation = animName;
            State.NowFrame = 0;
            State.LastFrameSecond = chrono::steady_clock::now();

            AnimationTracks[object] = State;
        }

        void StopTrack(const Inst& object) {
            AnimationTracks.erase(object);
        }

        void UpdateAnimation(const Inst& object) {
            auto StateEntry = AnimationTracks.find(object);
            if (StateEntry == AnimationTracks.end()) { return; }

            AnimationTrack& State = StateEntry->second;
            string AnimationName = State.NowAnimation;

            auto TimeNow = chrono::steady_clock::now();

            auto Elapsed = chrono::duration_cast<chrono::milliseconds>(TimeNow - State.LastFrameSecond).count() / 1000.0f; // Set a timer to find the end of the animation as time acculumates

            float FrameDuration = FrameDurations[AnimationName];

            if (Elapsed >= FrameDuration) {
                State.NowFrame++;
                State.LastFrameSecond = TimeNow;

                int TotalFrames = AnimationFrames[AnimationName].size();
                if (State.NowFrame >= TotalFrames) {
                    State.NowFrame = 0;
                }
            }
        }

        Spr GetFrameNow(const Inst& object) {
            auto StateEntry = AnimationTracks.find(object);
            if (StateEntry == AnimationTracks.end()) { return Spr{}; }

            AnimationTrack& State = StateEntry->second;
            string AnimationName = State.NowAnimation;
            string FrameName = AnimationFrames[AnimationName][State.NowFrame];

            vector<Spr> AssetsUnderName = Assets.GetAssetsInName(FrameName);
            if (!AssetsUnderName.empty()) {
                return AssetsUnderName[0];
            }

            return Spr{};
        }

        vector<Spr> GetFrames(const string& animName) {
            vector<Spr> Frames;

            auto NameEntry = AnimationFrames.find(animName);
            if (NameEntry != AnimationFrames.end()) {
                for (auto& FrameName : NameEntry->second) {
                    vector<Spr> AssetsUnderName = Assets.GetAssetsInName(FrameName);
                    if (!Assets.empty()) { Frames.push_back(AssetsUnderName[0]); }
                }
            }
            
            return Frames;
        }

        float GetFrameDuration(const string& animName) {
            auto NameEntry = FrameDurations.find(animName)

            if (NameEntry != FrameDurations.end()) {
                return NameEntry->second;
            }
            return 0.1f;
        }

        int GetFrameCount(const string& animName) {
            auto NameEntry = AnimationFrames.find(animName);

            if (NameEntry != AnimationFrames.end()) {
                return NameEntry->second.size();
            }

            return 0;
        }
};

#endif