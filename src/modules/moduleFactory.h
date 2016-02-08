//
//  moduleFactory.h
//  karmaMapper
//
//  Created by Daan de Lange on 5/2/16.
//
//  Allows a module to register and thus be instantiated from a string.
//  This class runs transparently in the background and you just have to add a register line in the Derived Base class to register it then extend this class with the karmaModule
//
//	Based on code from: http://gamedev.stackexchange.com/questions/17746/entity-component-systems-in-c-how-do-i-discover-types-and-construct-component/17759#17759

#pragma once

// Standard libraries
#include <map>
#include <string>
#include <utility>

class karmaModule;

namespace module
{
	namespace factory
	{
		typedef karmaModule* (*CreateModuleFunc)();
		typedef std::map<std::string, CreateModuleFunc> moduleRegistry;
		
		inline moduleRegistry& getModuleRegistry()
		{
			static moduleRegistry reg;
			return reg;
		}
		
		template<class T>
		karmaModule* createModule() {
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
				moduleRegistry& reg = getModuleRegistry();
				CreateModuleFunc func = createModule<T>;
				
				std::pair<moduleRegistry::iterator, bool> ret =
				reg.insert(moduleRegistry::value_type(name, func));
				
				if (ret.second == false) {
					// This means there already is a module registered to
					// this name. You should handle this error as you see fit.
				}
			}
			
			RegistryEntry(const RegistryEntry<T>&) = delete; // C++11 feature
			RegistryEntry& operator=(const RegistryEntry<T>&) = delete;
		};
		
	} // namespace factory
	
} // namespace module
