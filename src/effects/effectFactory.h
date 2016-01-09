//
//  effectFactory.h
//  karmaMapper
//
//  Created by Daan de Lange on 8/1/16.
//
//  Allows a effect to register and thus be instantiated from a string.
//  This class runs transparently in the background and you just have to add a register line in the Derived Base class to register it then extend this class with the basicEffect
//
//	Based on code from: http://gamedev.stackexchange.com/questions/17746/entity-component-systems-in-c-how-do-i-discover-types-and-construct-component/17759#17759

#pragma once

// Standard libraries
#include <map>
#include <string>
#include <utility>

class basicEffect;

namespace effect
{
	namespace factory
	{
		typedef basicEffect* (*CreateEffectFunc)();
		typedef std::map<std::string, CreateEffectFunc> effectRegistry;
		
		inline effectRegistry& getEffectRegistry()
		{
			static effectRegistry reg;
			return reg;
		}
		
		template<class T>
		basicEffect* createEffect() {
			return new T;
		}
		
		template<class T>
		struct RegistryEntry
		{
		public:
			static RegistryEntry<T>& Instance(const std::string& name)
			{
				// Because I use a singleton here, even though `effect_REGISTER`
				// is expanded in multiple translation units, the constructor
				// will only be executed once. Only this cheap `Instance` function
				// (which most likely gets inlined) is executed multiple times.
				
				static RegistryEntry<T> inst(name);
				return inst;
			}
			
		private:
			RegistryEntry(const std::string& name)
			{
				effectRegistry& reg = getEffectRegistry();
				CreateEffectFunc func = createEffect<T>;
				
				std::pair<effectRegistry::iterator, bool> ret =
				reg.insert(effectRegistry::value_type(name, func));
				
				if (ret.second == false) {
					// This means there already is a effect registered to
					// this name. You should handle this error as you see fit.
				}
			}
			
			RegistryEntry(const RegistryEntry<T>&) = delete; // C++11 feature
			RegistryEntry& operator=(const RegistryEntry<T>&) = delete;
		};
		
	} // namespace factory
	
} // namespace effect
