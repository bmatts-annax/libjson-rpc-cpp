/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    specificationparser.h
 * @date    12.03.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef JSONRPC_CPP_SPECIFICATIONPARSER_H
#define JSONRPC_CPP_SPECIFICATIONPARSER_H

#include "procedure.h"
#include "exception.h"

namespace jsonrpc {

    class SpecificationParser
    {
        public:
            static std::vector<Procedure> GetProceduresFromFile(const std::string& filename)    throw (JsonRpcException);
            static std::vector<Procedure> GetProceduresFromString(const std::string& spec)      throw (JsonRpcException);

        private:
            static void         GetProcedure    (Json::Value& val, Procedure &target);
            static void         GetMethod       (Json::Value& val, Procedure &target);
            static void         GetNotification (Json::Value& val, Procedure &target);
            static void         GetFileContent  (const std::string& filename, std::string& target);
            static jsontype_t   toJsonType      (Json::Value& val);

            static void         GetPositionalParameters (Json::Value &val, Procedure &target);
            static void         GetNamedParameters      (Json::Value &val, Procedure &target);

    };
}
#endif // JSONRPC_CPP_SPECIFICATIONPARSER_H