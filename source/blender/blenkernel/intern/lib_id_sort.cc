/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <algorithm>

#include "BLI_listbase.h"
#include "BLI_string.h"

#include "DNA_ID.h"
#include "DNA_listBase.h"

#include "BKE_lib_id.h"

extern "C" {

void id_sort_by_name(ListBase *lb, ID *id, ID *id_sorting_hint)
{
#define ID_SORT_STEP_SIZE 512

  ID *idtest;

  /* insert alphabetically */
  if (lb->first == lb->last) {
    return;
  }

  BLI_remlink(lb, id);

  /* Check if we can actually insert id before or after id_sorting_hint, if given. */
  if (!ELEM(id_sorting_hint, nullptr, id) && id_sorting_hint->lib == id->lib) {
    BLI_assert(BLI_findindex(lb, id_sorting_hint) >= 0);

    ID *id_sorting_hint_next = static_cast<ID *>(id_sorting_hint->next);
    if (BLI_strcasecmp(id_sorting_hint->name, id->name) < 0 &&
        (id_sorting_hint_next == nullptr || id_sorting_hint_next->lib != id->lib ||
         BLI_strcasecmp(id_sorting_hint_next->name, id->name) > 0)) {
      BLI_insertlinkafter(lb, id_sorting_hint, id);
      return;
    }

    ID *id_sorting_hint_prev = static_cast<ID *>(id_sorting_hint->prev);
    if (BLI_strcasecmp(id_sorting_hint->name, id->name) > 0 &&
        (id_sorting_hint_prev == nullptr || id_sorting_hint_prev->lib != id->lib ||
         BLI_strcasecmp(id_sorting_hint_prev->name, id->name) < 0)) {
      BLI_insertlinkbefore(lb, id_sorting_hint, id);
      return;
    }
  }

  /* Look for the last ID of the expected library.*/
  /* NOTE: We start from the end, because in typical 'heavy' case (insertion of lots of IDs at
   * once using the same base name), newly inserted items will generally be towards the end
   * (higher extension numbers). */
  for (idtest = static_cast<ID *>(lb->last); (idtest != nullptr) && (idtest->lib != id->lib);
       idtest = static_cast<ID *>(idtest->prev)) {
  }

  /* idtest can be null, expected library is not found,
   * or can be on the last id of the expected library.
   *
   * If expected library not found and if `id` is local,
   * all the items in the list are greater than the inserted one,
   * so we can put it at the start of the list. Or, if `id` is linked, it is the
   * first one of its library, and we can put it at the very end of the list.*/
  if (idtest == nullptr) {
    if (ID_IS_LINKED(id)) {
      BLI_addtail(lb, id);
    }
    else {
      BLI_addhead(lb, id);
    }
    return;
  }

  /* In case idtest is on the last id of the expected library, walk to the first ID in
   * the list that is smaller than the newly to be inserted ID but still part of the same
   * library.*/
  ID *item_array[ID_SORT_STEP_SIZE] = {nullptr};
  int item_array_index = ID_SORT_STEP_SIZE - 1;
  for (; (idtest) && (idtest->lib == id->lib); idtest = static_cast<ID *>(idtest->prev)) {
    item_array[item_array_index] = idtest;
    if (item_array_index == 0) {
      if (BLI_strcasecmp(idtest->name, id->name) <= 0) {
        break;
      }
      item_array_index = ID_SORT_STEP_SIZE;
    }
    item_array_index--;
  }

  ID **lower_bound = std::lower_bound(
      &item_array[item_array_index + 1],
      &item_array[ID_SORT_STEP_SIZE],
      id,
      [](ID *&a, ID *const &b) { return BLI_strcasecmp(a->name, b->name) <= 0; });

  if (lower_bound == &item_array[ID_SORT_STEP_SIZE]) {
    BLI_insertlinkafter(lb, item_array[ID_SORT_STEP_SIZE - 1], id);
  }
  else {
    BLI_insertlinkbefore(lb, *lower_bound, id);
  }
#undef ID_SORT_STEP_SIZE
}
}