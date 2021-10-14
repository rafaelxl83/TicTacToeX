#pragma once

#include "Threads.h"
#include "Utils.h"

#include <functional>
#include <vector>

class MessageBus
{
public:
	static MessageBus&							GetInstance()
	{
		static MessageBus instance;
		return instance;
	}

	template<typename T>
	void										SendToGamePlay(
													const T&									aMessage);

	template<typename T>
	void										SendToPlayers(
													const T&									aMessage);

	template<typename T>
	void										RegisterBoard(
													std::function<void(T)>						aCallback);

	template<typename T>
	void										RegisterPlayer(
													std::function<void(T)>						aCallback);

private:
												MessageBus() = default;
	
	template<typename T>
	static std::vector<std::function<void(T)>>	myGamePlayCallbacks;

	template<typename T>
	static std::vector<std::function<void(T)>>	myPlayersCallbacks;
};

template<typename T>
std::vector<std::function<void(T)>>	MessageBus::myGamePlayCallbacks;

template<typename T>
std::vector<std::function<void(T)>>	MessageBus::myPlayersCallbacks;

template<typename T>
void
MessageBus::SendToGamePlay(
	const T&							aMessage)
{
	auto& threads = Threads::GetInstance();
	
	for (auto& callback : myGamePlayCallbacks<T>)
	{
		threads.AddBoardWork([=](){ callback(aMessage); });
	}
}

template<typename T>
void
MessageBus::SendToPlayers(
	const T&							aMessage)
{
	auto& threads = Threads::GetInstance();
	
	for (auto& callback : myPlayersCallbacks<T>)
	{
		threads.AddPlayerWork([=](){ callback(aMessage); });
	}
}

template<typename T>
void
MessageBus::RegisterBoard(
	std::function<void(T)>				aCallback)
{
	myGamePlayCallbacks<T>.push_back(std::move(aCallback));
}

template<typename T>
void
MessageBus::RegisterPlayer(
	std::function<void(T)>				aCallback)
{
	myPlayersCallbacks<T>.push_back(std::move(aCallback));
}

#define REGISTER_BOARD(MessageType, Func) \
MessageBus::GetInstance().RegisterBoard<MessageType>([this](const MessageType& aMessage) { Func(aMessage); })

#define REGISTER_PLAYER(MessageType, Func) \
MessageBus::GetInstance().RegisterPlayer<MessageType>([this](const MessageType& aMessage) { Func(aMessage); })

#define SEND_TO_GAMEPLAY(Message) \
MessageBus::GetInstance().SendToGamePlay(Message)

#define SEND_TO_PLAYERS(Message) \
MessageBus::GetInstance().SendToPlayers(Message)
