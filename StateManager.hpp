#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <chrono>

using namespace std;

class StateManager {
    private:
        map<string, map<string, chrono::steady_clock::time_point>> States;
    public:
        template<typename Inst>

        void SetState(const Inst& object, string state, float duration) {
            auto TimeLeft = chrono::steady_clock::now() + chrono::milliseconds(static_cast<int>(duration * 1000));

            States[object][state] = TimeLeft; // The object holds state X for a duration of D.
        }

        template<typename Inst>

        void RemoveStates(const Inst& object, string state) {
            auto TheObject = States.find(object);

            if (TheObject != States.end()) {
                TheObject->second.erase(state); // Second is the inner map of the index where A and B in <a, <A, B>>

                if (TheObject->second.empty()) {
                    States.erase(TheObject);
                }
            }
        }

        template<typename Inst>

        bool GetState(const Inst& object, string state) {
            auto TheObject = States.find(object);

            if (TheObject == States.end()) { return false; }
            
            auto StateEntry = TheObject->second.find(state);

            if (StateEntry == TheObject->second.end()) { return false; };

            if (chrono::steady_clock::now() > StateEntry->second) {
                TheObject->second.erase(StateEntry);
                return false;
            }

            return true;
        }

        template<typename Inst>

        vector<string> ReturnStates(const Inst& object) {
            vector<string> AllStates = {};

            auto ObjectEntry = States.find(object);

            if (ObjectEntry == States.end()) { cout << "ERROR: Invalid parameter 'Object' of ReturnStates()" << endl; return AllStates; }

            auto& StateEntry = ObjectEntry->second;

            if (StateEntry.empty()) { return AllStates; }

            auto Now = chrono::steady_clock::now();

            // Map is basically a state reference.

            for (auto Map = StateEntry.begin(); Map != StateEntry.end();) {
                if (Now > Map->second) {
                    Map = StateEntry.erase(Map);
                } else {
                    AllStates.push_back(Map->first);
                    ++Map; // Move to next state.
                }
            }

            return AllStates;
        }
};

#endif