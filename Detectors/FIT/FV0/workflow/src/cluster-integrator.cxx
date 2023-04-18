// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "FITWorkflow/FITIntegrateClusterSpec.h"
#include "FITWorkflow/FITIntegrateClusterWriterSpec.h"
#include "CommonUtils/ConfigurableParam.h"
#include "Framework/ConfigParamSpec.h"
#include "DataFormatsFV0/RecPoints.h"

using namespace o2::framework;

void customize(std::vector<o2::framework::ConfigParamSpec>& workflowOptions)
{
  std::vector<ConfigParamSpec> options{
    ConfigParamSpec{"configKeyValues", VariantType::String, "", {"Semicolon separated key=value strings"}},
    {"min-NChan", VariantType::Int, 2, {"Minimum NChan signal required to avoid noise"}},
    {"min-Ampl", VariantType::Int, 2, {"Minimum Ampl signal required to avoid noise"}},
    {"disable-root-output", VariantType::Bool, false, {"disable root-files output writers"}}};
  std::swap(workflowOptions, options);
}

#include "Framework/runDataProcessing.h"

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  WorkflowSpec wf;
  o2::conf::ConfigurableParam::updateFromString(cfgc.options().get<std::string>("configKeyValues"));
  const bool disableWriter = cfgc.options().get<bool>("disable-root-output");
  const int minNChan = cfgc.options().get<int>("min-NChan");
  const int minAmpl = cfgc.options().get<int>("min-Ampl");
  wf.emplace_back(o2::fit::getFITIntegrateClusterSpec<o2::fv0::RecPoints>(disableWriter, minNChan, minAmpl));
  if (!disableWriter) {
    wf.emplace_back(o2::fit::getFITIntegrateClusterWriterSpec<o2::fv0::RecPoints>());
  }
  return wf;
}
