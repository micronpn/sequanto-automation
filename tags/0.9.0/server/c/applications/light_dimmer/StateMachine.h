
#ifndef SEQUANTO_STATE_MACHINE_H_
#define SEQUANTO_STATE_MACHINE_H_

template<typename TState, typename TMessage>
class StateMachine
{
private:
   TState m_currentState;
   
protected:
   void setState ( TState _state )
   {
      m_currentState = _state;
   }
   
public:
   StateMachine ( TState _initialState )
   {
      m_currentState = _initialState;
   }

   TState state () const
   {
      return m_currentState;
   }
};

#endif /* SEQUANTO_STATE_MACHINE_H_ */
