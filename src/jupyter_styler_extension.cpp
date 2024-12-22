#define DUCKDB_EXTENSION_MAIN

#include "jupyter_styler_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void JupyterStylerScalarFun(DataChunk &args, ExpressionState &state,
                                   Vector &result) {
  auto &name_vector = args.data[0];
  UnaryExecutor::Execute<string_t, string_t>(
      name_vector, result, args.size(), [&](string_t name) {
        return StringVector::AddString(result, "JupyterStyler " +
                                                   name.GetString() + " 🐥");
        ;
      });
}

inline void JupyterStylerOpenSSLVersionScalarFun(DataChunk &args,
                                                 ExpressionState &state,
                                                 Vector &result) {
  auto &name_vector = args.data[0];
  UnaryExecutor::Execute<string_t, string_t>(
      name_vector, result, args.size(), [&](string_t name) {
        return StringVector::AddString(result,
                                       "JupyterStyler " + name.GetString() +
                                           ", my linked OpenSSL version is " +
                                           OPENSSL_VERSION_TEXT);
        ;
      });
}

static void LoadInternal(DatabaseInstance &instance) {
  // Register a scalar function
  auto jupyter_styler_scalar_function =
      ScalarFunction("jupyter_styler", {LogicalType::VARCHAR},
                     LogicalType::VARCHAR, JupyterStylerScalarFun);
  ExtensionUtil::RegisterFunction(instance, jupyter_styler_scalar_function);

  // Register another scalar function
  auto jupyter_styler_openssl_version_scalar_function = ScalarFunction(
      "jupyter_styler_openssl_version", {LogicalType::VARCHAR},
      LogicalType::VARCHAR, JupyterStylerOpenSSLVersionScalarFun);
  ExtensionUtil::RegisterFunction(
      instance, jupyter_styler_openssl_version_scalar_function);
}

void JupyterStylerExtension::Load(DuckDB &db) { LoadInternal(*db.instance); }
std::string JupyterStylerExtension::Name() { return "jupyter_styler"; }

std::string JupyterStylerExtension::Version() const {
#ifdef EXT_VERSION_JUPYTER_STYLER
  return EXT_VERSION_JUPYTER_STYLER;
#else
  return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void jupyter_styler_init(duckdb::DatabaseInstance &db) {
  duckdb::DuckDB db_wrapper(db);
  db_wrapper.LoadExtension<duckdb::JupyterStylerExtension>();
}

DUCKDB_EXTENSION_API const char *jupyter_styler_version() {
  return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
