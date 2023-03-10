#pragma once
/*
 * Copyright (C) 2020-2021 LEIDOS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
#include <lanelet2_core/primitives/RegulatoryElement.h>
#include <boost/algorithm/string.hpp>
#include <unordered_set>

namespace lanelet
{
/**
 * @brief Represents a virtual stop and wait line horizontally laying on the roadway. It indicates whether a given participant 
 * should stop and wait momentarily before passing the line. General usage is as a stop line that is not represented by an actual
 * physical roadway object. By default, it does not apply to all objects except the ones in participant list.
 *
 * A StopRule is created from a list of contiguous LineString3d and participants who should stop and wait before crossing.
 * The object is agnostic to the line's invertedness.
 *
 * @ingroup RegulatoryElementPrimitives
 * @ingroup Primitives
 */
class StopRule : public RegulatoryElement
{
public:
  static constexpr char RuleName[] = "stop_rule";
  static constexpr char Participants[] = "participants";
  std::unordered_set<std::string> participants_;

  /**
   * @brief get the list of contigious line strings that form this stop and wait line
   * @return the lines as a list of line strings
   */
  ConstLineStrings3d stopAndWaitLine() const;
  /**
   * @brief Same as ConstLineStrings3d stopAndWaitLine() const but without the const modifier
   * However, the implementation of this method is expected to be const
   */
  LineStrings3d stopAndWaitLine();

  /**
   * @brief Returns true if the provided participant should stop and wait before crossing the line
   *
   * @param participant The string classification of the participant type
   *
   * @return True if participant should stop and wait momentarily before the line
   */
  bool appliesTo(const std::string& participant) const;

  /**
   * @brief Helper function to match a given line with a stop and wait line regulatory element then determine if it applies to
   * the given participant
   *
   * The set of line strings contained in each of the provided stop and wait lines is searched until a sub-line is found that
   * matches the provided lanelet or area line. The returned value indicates if the rule is applied to the participant
   *
   * @param line The line to stop before
   * @param stopAndWaitLines The set of possible stop lines which this line might be a part of
   * @param participant The participant being evaluated
   *
   * @return True if the rule is applied to the participant or if none of the stopAndWaitLines match the
   * provided line
   */
  static bool appliesTo(const ConstLineString3d& line,
                            const std::vector<std::shared_ptr<const StopRule>>& stopAndWaitLines,
                            const std::string& participant);

  static bool appliesTo(const ConstLineString3d& line,
                            const std::vector<std::shared_ptr<StopRule>>& stopAndWaitLines,
                            const std::string& participant);

  /**
   * @brief Constructor defined to support loading from lanelet files
   */
  explicit StopRule(const lanelet::RegulatoryElementDataPtr& data);

  /**
   * @brief Static helper function that creates a stop line data object based on the provided inputs
   *
   * @param id The lanelet::Id of this element
   * @param stopAndWaitLine The line strings which represent this regularoty elements geometry
   * @param participants The set of participants which this rule applies to
   *
   * @return RegulatoryElementData containing all the necessary information to construct a stop rule
   */
  static std::unique_ptr<lanelet::RegulatoryElementData> buildData(Id id, LineStrings3d stopAndWaitLine,
                                                     std::vector<std::string> participants);

protected:
  // the following lines are required so that lanelet2 can create this object when loading a map with this regulatory
  // element
  friend class RegisterRegulatoryElement<StopRule>;
};

// Convenience Ptr Declarations
using StopRulePtr = std::shared_ptr<StopRule>;
using StopRuleConstPtr = std::shared_ptr<const StopRule>;

}  // namespace lanelet
