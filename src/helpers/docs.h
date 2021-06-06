#pragma once

#include "main.h"
#include "cpp-sdk/SDK.h"
#include "Log.h"
#include "angelscript/include/angelscript.h"
#include "../resource.h"
#ifdef AS_GENERATE_DOCUMENTATION
    #include <chrono>
    #include <ctime>
    #include <iostream>
    #include <fstream>
#endif

#define PAD_SPACE "    "

// The whole docs generator is a mess, but it does the job

namespace Helpers
{
    class DocsGenerator
    {
        std::string                                                       moduleName;
        std::vector<std::pair<std::string, std::string>>                  declarations;
        std::vector<std::pair<std::string, std::string>>                  variables;
        std::vector<std::pair<std::string, std::string>>                  funcDefs;
        std::vector<std::pair<std::string, std::string>>                  typeDefs;
        std::vector<std::pair<std::string, std::string>>                  eventDeclarations;
        std::vector<std::tuple<std::string, std::string, std::string>>    objectTypes;
        std::vector<std::pair<std::string, std::string>>                  enumTypes;
        std::unordered_multimap<std::string, std::pair<std::string, int>> enumValues;
        std::unordered_multimap<std::string, std::string>                 objectDeclarations;
        std::unordered_multimap<std::string, std::string>                 objectConstructors;
        std::unordered_multimap<std::string, std::string>                 objectMethods;

    public:
        DocsGenerator(std::string moduleName) : moduleName(moduleName){};

        void PushDeclaration(std::string decl, std::string desc)
        {
#ifdef AS_GENERATE_DOCUMENTATION
            declarations.push_back(std::pair(decl, desc));
#endif
        }
        void PushVariable(std::string type, std::string prop)
        {
#ifdef AS_GENERATE_DOCUMENTATION
            variables.push_back({ type, prop });
#endif
        }
        void PushFuncDef(std::string funcdef, std::string desc)
        {
#ifdef AS_GENERATE_DOCUMENTATION
            funcDefs.push_back(std::pair(funcdef, desc));
#endif
        }
        void PushTypedef(std::string type, std::string original)
        {
#ifdef AS_GENERATE_DOCUMENTATION
            typeDefs.push_back(std::pair(type, original));
#endif
        }
        void PushEventDeclaration(std::string funcDef, std::string globalFunc)
        {
#ifdef AS_GENERATE_DOCUMENTATION
            eventDeclarations.push_back(std::pair(funcDef.insert(0, "funcdef "), globalFunc));
#endif
        }
        void PushObjectType(std::string name, std::string desc, std::string base)
        {
#ifdef AS_GENERATE_DOCUMENTATION
            objectTypes.push_back(std::tuple(name, desc, base));
#endif
        }
        void PushObjectProperty(std::string object, std::string propertyDecl)
        {
#ifdef AS_GENERATE_DOCUMENTATION
            objectDeclarations.insert({ object, propertyDecl });
#endif
        }
        void PushObjectConstructor(std::string object, std::string constructorDecl)
        {
#ifdef AS_GENERATE_DOCUMENTATION
            objectConstructors.insert({ object, constructorDecl });
#endif
        }
        void PushObjectMethod(std::string object, std::string methodDecl)
        {
#ifdef AS_GENERATE_DOCUMENTATION
            objectMethods.insert({ object, methodDecl });
#endif
        }
        void PushEnumValue(std::string enumName, std::string name, int value)
        {
#ifdef AS_GENERATE_DOCUMENTATION
            enumValues.insert({ enumName, { name, value } });
#endif
        }
        void PushEnumType(std::string name, std::string desc)
        {
#ifdef AS_GENERATE_DOCUMENTATION
            enumTypes.push_back({ name, desc });
#endif
        }

        void Generate()
        {
#ifdef AS_GENERATE_DOCUMENTATION
            std::stringstream stream;

            // Add generation date to top of file
            std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            char*       timeStr     = new char[40];
            ctime_s(timeStr, 40, &currentTime);
            stream << "// Generated on " << timeStr << "\n";

            // Add namespace
            stream << "namespace " << moduleName << "\n";
            stream << "{"
                   << "\n";

            // Add func defs
            if(funcDefs.size() > 0)
            {
                stream << PAD_SPACE << "// ********** Funcdefs **********"
                       << "\n";
                for(auto def : funcDefs)
                {
                    stream << "\n";
                    stream << PAD_SPACE << "// " << def.second << "\n";
                    stream << PAD_SPACE << "funcdef " << def.first << ";"
                           << "\n";
                }

                stream << "\n";
            }

            // Add typedefs
            if(typeDefs.size() > 0)
            {
                stream << PAD_SPACE << "// ********** Typedefs **********"
                       << "\n";
                for(auto def : typeDefs)
                {
                    stream << "\n";
                    stream << PAD_SPACE << "typedef " << def.second << " " << def.first << ";"
                           << "\n";
                }
                stream << "\n";
            }

            // Add enums
            if(enumTypes.size() > 0)
            {
                stream << PAD_SPACE << "// ********** Enums **********"
                       << "\n";
                for(auto enumType : enumTypes)
                {
                    stream << "\n";
                    stream << PAD_SPACE << "// " << enumType.second << "\n";
                    stream << PAD_SPACE << "enum " << enumType.first << "\n";
                    stream << PAD_SPACE << "{"
                           << "\n";
                    for(auto value : enumValues)
                    {
                        if(value.first != enumType.first) continue;
                        stream << PAD_SPACE << PAD_SPACE << value.second.first << " = " << std::to_string(value.second.second) << ",\n";
                    }
                    stream << PAD_SPACE << "};"
                           << "\n";
                }

                stream << "\n";
            }

            // Add variables
            if(variables.size() > 0)
            {
                stream << PAD_SPACE << "// ********** Global variables **********"
                       << "\n";
                for(auto variable : variables)
                {
                    stream << "\n";
                    stream << PAD_SPACE << variable.first << " " << variable.second << ";"
                           << "\n";
                }

                stream << "\n";
            }

            // Add function declarations
            if(declarations.size() > 0)
            {
                stream << PAD_SPACE << "// ********** Functions **********"
                       << "\n";
                for(auto decl : declarations)
                {
                    stream << "\n";
                    stream << PAD_SPACE << "// " << decl.second << "\n";
                    stream << PAD_SPACE << decl.first << ";"
                           << "\n";
                }

                stream << "\n";
            }

            // Add event declarations
            if(eventDeclarations.size() > 0)
            {
                stream << PAD_SPACE << "// ********** Events **********"
                       << "\n";
                for(auto decl : eventDeclarations)
                {
                    stream << "\n";
                    stream << PAD_SPACE << decl.first << ";"
                           << "\n";
                    stream << PAD_SPACE << decl.second << ";"
                           << "\n";
                }

                stream << "\n";
            }

            // Add object types
            if(objectTypes.size() > 0)
            {
                stream << PAD_SPACE << "// ********** Objects **********\n";
                for(auto obj : objectTypes)
                {
                    auto [name, desc, base] = obj;
                    stream << "\n";
                    stream << PAD_SPACE << "// " << desc << "\n";
                    stream << PAD_SPACE << "class " << name;
                    if(base != "") stream << " : " << base;
                    stream << "\n";
                    stream << PAD_SPACE << "{\n";
                    for(auto kv : objectDeclarations)
                    {
                        if(kv.first != name) continue;
                        stream << PAD_SPACE << PAD_SPACE << CleanDeclaration(kv.second) << ";\n";
                    }
                    stream << "\n";
                    for(auto kv : objectConstructors)
                    {
                        if(kv.first != name) continue;
                        stream << PAD_SPACE << PAD_SPACE << name << "(" << kv.second << ");\n";
                    }
                    for(auto kv : objectMethods)
                    {
                        if(kv.first != name) continue;
                        stream << PAD_SPACE << PAD_SPACE << kv.second << ";\n";
                    }
                    stream << PAD_SPACE << "};\n";
                }
            }

            // Close namespace
            stream << "}"
                   << "\n";

            // Write the docs to file
            std::ofstream file;
            file.open(moduleName + "Docs.as");
            file << stream.str();
            file.close();
            Log::Colored << "~y~Generated ~w~" << moduleName << " ~y~docs" << Log::Endl;
#endif
        }

        static std::string CleanDeclaration(std::string& decl)
        {
            static std::vector<char> invalidChars = { '+' };

            std::string output;
            output.reserve(decl.size());
            for(auto invalidChar : invalidChars)
            {
                for(size_t i = 0; i < decl.size(); i++)
                {
                    if(decl[i] != invalidChar) output += decl[i];
                }
            }
            return output;
        }
    };
}  // namespace Helpers
