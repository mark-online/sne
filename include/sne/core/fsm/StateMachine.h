#pragma once

#include "State.h"
#include <boost/noncopyable.hpp>

namespace sne { namespace core { namespace fsm {

class Event;

/** @addtogroup Fsm
* @{
*/

/**
 * @class StateMachine
 */
template <typename Owner>
class StateMachine : public boost::noncopyable
{
public:
    StateMachine(Owner& owner) :
        owner_(owner),
        currentState_(nullptr),
        previousState_(nullptr),
        globalState_(nullptr) {}

    virtual ~StateMachine() {}

public:
    void setCurrentState(State<Owner>& state) {
        currentState_ = &state;
    }

    void setGlobalState(State<Owner>& state) {
        globalState_ = &state;
    }

    void setPreviousState(State<Owner>& state) {
        previousState_ = &state;
    }

public:
    void update(msec_t diff) const {
        if (globalState_ != nullptr) {
            globalState_->update(owner_, diff);
        }

        if (currentState_ != nullptr) {
            currentState_->update(owner_, diff);
        }
    }

    bool handleEvent(Event& event) const {
        if (currentState_ != nullptr) {
            if (currentState_->handleEvent(owner_, event)) {
                return true;
            }
        }

        if (globalState_ != nullptr) {
            if (globalState_->handleEvent(owner_, event)) {
                return true;
            }
        }

        return false;
    }

    bool changeState(State<Owner>& newState) {
        if (isInState(newState)) {
            return false;
        }

        previousState_ = currentState_;

        if (currentState_ != nullptr) {
            currentState_->exit(owner_);
        }

        currentState_ = &newState;
        currentState_->entry(owner_);

        return true;
    }

    void revertToPreviousState() {
        assert(previousState_ != nullptr);
        changeState(previousState_);
    }

    bool isInState(const State<Owner>& state) const {
        return currentState_ == &state;
    }

    State<Owner>* getCurrentState() const {
        return currentState_;
    }
    
    State<Owner>* getGlobalState() const {
        return globalState_;
    }

    State<Owner>* getPreviousState() const {
        return previousState_;
    }

public:
    Owner& getOwner() {
        return owner_;
    }

    const Owner& getOwner() const {
        return owner_;
    }

private:
    Owner& owner_;

    State<Owner>* currentState_;
    State<Owner>* previousState_;

    //this is called every time the FSM is updated
    State<Owner>* globalState_;
};

/** @} */ // addtogroup Fsm

}}} // namespace sne { namespace core { namespace fsm {
