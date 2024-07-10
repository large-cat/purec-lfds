cmake_minimum_required(VERSION 3.10)
project(purec-lfds)

# 设置C++标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/inc)
# 添加可执行文件
add_library(purec-lfds 
    src/lfds_misc/lfds_misc_query.c
    src/lfds_misc/lfds_misc_internal.h
    src/lfds_misc/lfds_misc_globals.c
    src/lfds_misc/lfds_misc_internal_backoff_init.c
    src/lfds_btree_addonly_unbalanced/lfds_btree_addonly_unbalanced_query.c
    src/lfds_btree_addonly_unbalanced/lfds_btree_addonly_unbalanced_get.c
    src/lfds_btree_addonly_unbalanced/lfds_btree_addonly_unbalanced_internal.h
    src/lfds_btree_addonly_unbalanced/lfds_btree_addonly_unbalanced_cleanup.c
    src/lfds_btree_addonly_unbalanced/lfds_btree_addonly_unbalanced_init.c
    src/lfds_btree_addonly_unbalanced/lfds_btree_addonly_unbalanced_insert.c
    src/lfds_queue_unbounded_multi2multi/lfds_queue_unbounded_multi2multi_query.c
    src/lfds_queue_unbounded_multi2multi/lfds_queue_unbounded_multi2multi_internal.h
    src/lfds_queue_unbounded_multi2multi/lfds_queue_unbounded_multi2multi_dequeue.c
    src/lfds_queue_unbounded_multi2multi/lfds_queue_unbounded_multi2multi_init.c
    src/lfds_queue_unbounded_multi2multi/lfds_queue_unbounded_multi2multi_cleanup.c
    src/lfds_queue_unbounded_multi2multi/lfds_queue_unbounded_multi2multi_enqueue.c
    src/lfds_queue_bounded_multi2multi/lfds_queue_bounded_multi2multi_query.c
    src/lfds_queue_bounded_multi2multi/lfds_queue_bounded_multi2multi_internal.h
    src/lfds_queue_bounded_multi2multi/lfds_queue_bounded_multi2multi_init.c
    src/lfds_queue_bounded_multi2multi/lfds_queue_bounded_multi2multi_cleanup.c
    src/lfds_queue_bounded_multi2multi/lfds_queue_bounded_multi2multi_dequeue.c
    src/lfds_queue_bounded_multi2multi/lfds_queue_bounded_multi2multi_enqueue.c
    src/lfds_prng/lfds_prng_init.c
    src/lfds_prng/lfds_prng_internal.h
    src/lfds_hash_addonly/lfds_hash_addonly_iterate.c
    src/lfds_hash_addonly/lfds_hash_addonly_insert.c
    src/lfds_hash_addonly/lfds_hash_addonly_get.c
    src/lfds_hash_addonly/lfds_hash_addonly_cleanup.c
    src/lfds_hash_addonly/lfds_hash_addonly_internal.h
    src/lfds_hash_addonly/lfds_hash_addonly_init.c
    src/lfds_hash_addonly/lfds_hash_addonly_query.c
    src/lfds_stack/lfds_stack_query.c
    src/lfds_stack/lfds_stack_pop.c
    src/lfds_stack/lfds_stack_internal.h
    src/lfds_stack/lfds_stack_cleanup.c
    src/lfds_stack/lfds_stack_init.c
    src/lfds_stack/lfds_stack_push.c
    src/liblfds_internal.h
    src/lfds_queue_bounded_single2single/lfds_queue_bounded_single2single_init.c
    src/lfds_queue_bounded_single2single/lfds_queue_bounded_single2single_query.c
    src/lfds_queue_bounded_single2single/lfds_queue_bounded_single2single_dequeue.c
    src/lfds_queue_bounded_single2single/lfds_queue_bounded_single2single_cleanup.c
    src/lfds_queue_bounded_single2single/lfds_queue_bounded_single2single_internal.h
    src/lfds_queue_bounded_single2single/lfds_queue_bounded_single2single_enqueue.c
    src/lfds_list_addonly_single_order/lfds_list_addonly_single_order_init.c
    src/lfds_list_addonly_single_order/lfds_list_addonly_single_order_query.c
    src/lfds_list_addonly_single_order/lfds_list_addonly_single_order_insert.c
    src/lfds_list_addonly_single_order/lfds_list_addonly_single_order_cleanup.c
    src/lfds_list_addonly_single_order/lfds_list_addonly_single_order_get.c
    src/lfds_list_addonly_single_order/lfds_list_addonly_single_order_internal.h
    src/lfds_ringbuffer/lfds_ringbuffer_init.c
    src/lfds_ringbuffer/lfds_ringbuffer_cleanup.c
    src/lfds_ringbuffer/lfds_ringbuffer_query.c
    src/lfds_ringbuffer/lfds_ringbuffer_read.c
    src/lfds_ringbuffer/lfds_ringbuffer_write.c
    src/lfds_ringbuffer/lfds_ringbuffer_internal.h
    src/lfds_freelist/lfds_freelist_push.c
    src/lfds_freelist/lfds_freelist_internal.h
    src/lfds_freelist/lfds_freelist_pop.c
    src/lfds_freelist/lfds_freelist_query.c
    src/lfds_freelist/lfds_freelist_cleanup.c
    src/lfds_freelist/lfds_freelist_init.c
    src/lfds_list_addonly_single_unorder/lfds_list_addonly_single_unorder_cleanup.c
    src/lfds_list_addonly_single_unorder/lfds_list_addonly_single_unorder_init.c
    src/lfds_list_addonly_single_unorder/lfds_list_addonly_single_unorder_get.c
    src/lfds_list_addonly_single_unorder/lfds_list_addonly_single_unorder_insert.c
    src/lfds_list_addonly_single_unorder/lfds_list_addonly_single_unorder_query.c
    src/lfds_list_addonly_single_unorder/lfds_list_addonly_single_unorder_internal.h
)

# 添加链接的库
# target_link_libraries(my_executable
#     pthread  # 如果需要链接pthread库，这里作为示例
# )
