#ifndef CROWD_SIM_STATE_TABLE__H
#define CROWD_SIM_STATE_TABLE__H

#include <traffic_editor/crowd_sim/crowd_sim_impl.h>
#include "crowd_sim_table_base.h"

using namespace crowd_sim;

class StatesTab : public CrowdSimTableBase
{
public:
  static std::shared_ptr<StatesTab> init_and_make(
    CrowdSimImplPtr crowd_sim_impl);

  StatesTab(CrowdSimImplPtr crowd_sim_impl, const QStringList& labels)
  : CrowdSimTableBase(crowd_sim_impl, labels)
  {
    _cache = get_impl()->get_states();
  }
  ~StatesTab() {}

  int get_cache_size() const override
  {
    return static_cast<int>(_cache.size());
  }
  void list_item_in_cache() override;
  void save() override;
  void save_to_impl() override;
  void add_button_click() override;
  void delete_button_click(size_t row_number) override;

private:
  std::vector<crowd_sim::State> _cache;

  void _list_goal_sets_in_combo(QComboBox* comboBox,
    size_t current_goal_set_id);
  void _list_final_states_in_combo(QComboBox* comboBox, bool current_state);
  void _list_navmesh_file_in_combo(QComboBox* comboBox,
    std::string navmesh_filename);
};

#endif