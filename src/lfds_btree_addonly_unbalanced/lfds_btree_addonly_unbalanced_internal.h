/***** the library-wide header file *****/
#include "../liblfds_internal.h"

/***** enums *****/
enum lfds_btree_au_move
{
  LFDS_BTREE_AU_MOVE_INVALID,
  LFDS_BTREE_AU_MOVE_SMALLEST_FROM_RIGHT_CHILD,
  LFDS_BTREE_AU_MOVE_LARGEST_FROM_LEFT_CHILD,
  LFDS_BTREE_AU_MOVE_GET_PARENT,
  LFDS_BTREE_AU_MOVE_MOVE_UP_TREE
};

enum lfds_btree_au_delete_action
{
  LFDS_BTREE_AU_DELETE_SELF,
  LFDS_BTREE_AU_DELETE_SELF_REPLACE_WITH_LEFT_CHILD,
  LFDS_BTREE_AU_DELETE_SELF_REPLACE_WITH_RIGHT_CHILD,
  LFDS_BTREE_AU_DELETE_MOVE_LEFT
};

/***** private prototypes *****/

