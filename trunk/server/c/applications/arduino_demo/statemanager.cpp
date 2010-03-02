#include "common.h"

void StateManager::ChangeState(STATE_FCN newState)
{
	m_newState = newState;
}

void StateManager::HandleKeyPressed(char key)
{
	if (m_currentState != null) m_currentState(KEY_PRESSED, key, millis(), &m_save);
}

void StateManager::HandleKeyRepeated(char key, unsigned long time)
{
	if (m_currentState != null) m_currentState(KEY_REPEATED, key, time, &m_save);
}

void StateManager::HandleKeyReleased(char key, unsigned long time)
{
	if (m_currentState != null) m_currentState(KEY_RELEASED, key, time, &m_save);
}

void StateManager::HandleSecondTick()
{
	if (m_currentState != null) m_currentState(SECOND_TICK, 0, 0, &m_save);
}

void StateManager::HandleMinuteTick()
{
	if (m_currentState != null) m_currentState(MINUTE_TICK, 0, 0, &m_save);
}

void StateManager::OnIdle()
{
	if (m_newState != null)
	{
		m_lastState = m_currentState;
		m_currentState = m_newState;
		m_newState = null;
		if (m_lastState != null) m_lastState(STATE_LEAVE, 0, 0, &m_save);
		m_save = null;
		if (m_currentState != null) m_currentState(STATE_ENTRY, 0, 0, &m_save);
	}
}

