#include "strata/workload.h"
strata::Workload::Workload(std::string id, std::string tenant_id, ResourceVector request)
    : id_(id), tenant_id_(tenant_id), request_(request) {}

const std::string& strata::Workload::id() const { return id_; }
const std::string& strata::Workload::tenant_id() const { return tenant_id_; }
const strata::ResourceVector& strata::Workload::request() const { return request_; }