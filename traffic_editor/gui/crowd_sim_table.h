#ifndef CROWD_SIM_TABLE__H
#define CROWD_SIM_TABLE__H

#include <vector>
#include <string>
#include <set>

#include <QTableWidget>
#include <QtWidgets>

#include <table_list.h>
#include <project.h>
#include <traffic_editor/crowd_sim_impl.h>

class CrowdSimTable : public TableList
{
    Q_OBJECT;

public:
    CrowdSimTable(const Project& input_project);
    ~CrowdSimTable();

    void update();
    void update_goal_area();
    void update_navmesh_level();
    void update_external_agent_from_spawn_point();
    void update_external_agent_state();

private:
    std::vector<std::string> required_components { 
        "GoalSets", 
        "States", 
        "Transitions", 
        "AgentProfiles", 
        "AgentGroups", 
        "ModelTypes"};
    std::set<std::string> goal_areas;

    QTableWidgetItem* enable_crowd_sim_name_item;
    QCheckBox* enable_crowd_sim_checkbox;
    QTableWidgetItem* update_time_step_name_item;
    QLineEdit* update_time_step_value_item;

    CrowdSimImplPtr crowd_sim_impl;
    const Project& project;
};

#endif