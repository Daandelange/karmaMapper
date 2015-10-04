//
//  shapeFactory.h
//  karmaMapper
//
//  Created by Daan de Lange on 22/3/14.
//
//  Allows a shape to register and thus be instantiated from a string.
//  This class runs transparently in the background and you just have to add a register line in the Derived Base class to register it then extend this class with the basicShape
//
//	See: http://gamedev.stackexchange.com/questions/17746/entity-component-systems-in-c-how-do-i-discover-types-and-construct-component/17759#17759

#pragma once

// Standard libraries
#include <map>
#include <string>
#include <utility>

class basicShape;

namespace shape
{
	namespace factory
	{
		typedef basicShape* (*CreateShapeFunc)();
		typedef std::map<std::string, CreateShapeFunc> shapeRegistry;
		
		inline shapeRegistry& getShapeRegistry()
		{
			static shapeRegistry reg;
			return reg;
		}
		
		template<class T>
		basicShape* createShape() {
			return new T;
		}
		
		template<class T>
		struct RegistryEntry
		{
		public:
			static RegistryEntry<T>& Instance(const std::string& name)
			{
				// Because I use a singleton here, even though `SHAPE_REGISTER`
				// is expanded in multiple translation units, the constructor
				// will only be executed once. Only this cheap `Instance` function
				// (which most likely gets inlined) is executed multiple times.
				
				static RegistryEntry<T> inst(name);
				return inst;
			}
			
		private:
			RegistryEntry(const std::string& name)
			{
				shapeRegistry& reg = getShapeRegistry();
				CreateShapeFunc func = createShape<T>;
				
				std::pair<shapeRegistry::iterator, bool> ret =
				reg.insert(shapeRegistry::value_type(name, func));
				
				if (ret.second == false) {
					// This means there already is a shape registered to
					// this name. You should handle this error as you see fit.
				}
			}
			
			RegistryEntry(const RegistryEntry<T>&) = delete; // C++11 feature
			RegistryEntry& operator=(const RegistryEntry<T>&) = delete;
		};
		
	} // namespace factory
	
} // namespace shape
