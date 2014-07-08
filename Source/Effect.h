#pragma once

#include "State.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Color.hpp>
#include <functional>
#include <vector>
#include <string>

enum EffectType
{
	FADE,
	SCREEN_SHAKE
};

/*	An effect is mostly used as a fading effect.
	It has a unique callback function after the
	fading reaches the middle and works very smooth.
	Effects are only used with the effect manager.
*/
class Effect
{
	public:
		typedef std::function<void()> Callback;

	public:
		Effect(EffectType effectType);
		
		void update(sf::Time elapsedTime, sf::RenderTexture& overlayTexture);
		void setCallback(Callback callback);
		void executeTrigger();
		
		bool isDone() const;
		bool canExecuteTrigger() const;

	private:
		Callback callback;
		EffectType effectType;
		float alphaValue;
		float fadeInRatio;
		float fadeOutRatio;
		float duration;
		bool executeOnce;
		bool reachPoint;
		bool done;
};