#ifndef MSGBUS_PROTOBUF_H
#define MSGBUS_PROTOBUF_H

#define PB_MSGID 1

#include <pb.h>
#include <msgbus/messagebus.h>
#include <msgbus/posix/port.h>

#include <unistd.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const pb_field_t* fields;
    uint32_t msgid;
    messagebus_watcher_t udp_watcher;
} topic_metadata_t;

#define TOPIC_DECL(name, type)                                 \
    struct {                                                   \
        messagebus_topic_t topic;                              \
        condvar_wrapper_t var;                                 \
        type value;                                            \
        topic_metadata_t metadata;                             \
    } name = {                                                 \
        _MESSAGEBUS_TOPIC_DATA(name.topic,                     \
                               name.var,                       \
                               name.var,                       \
                               &name.value,                    \
                               sizeof(type),                   \
                               name.metadata),                 \
        {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER}, \
        type##_init_default,                                   \
        {                                                      \
            type##_fields,                                     \
            type##_msgid,                                      \
            {NULL, NULL},                                      \
        },                                                     \
    }

#define _MESSAGEBUS_TOPIC_DATA(topic, lock, condvar, buffer, buffer_size, metadata) \
    {                                                                               \
        buffer, buffer_size, &lock, &condvar, "", 0, NULL, NULL, &metadata, {0},    \
    }

/* Wraps the topic information in a header (in protobuf format) to be sent over
 * UDP or logged to disk or whatever.
 *
 * The scratch buffer must be big enough to hold the topic's content.
 *
 * @return The message size in bytes, or zero if there was an error.
 */
size_t messagebus_encode_topic_message(messagebus_topic_t* topic,
                                       uint8_t* buf,
                                       size_t buf_len,
                                       uint8_t* scratch,
                                       size_t scratch_len);

/** Takes a topic information with a header and injects it into the
 * corresponding topic.
 *
 * @warning This functions is not thread safe due to the use of a static buffer.
 */
void messagebus_inject_encoded_message(messagebus_t* bus, uint8_t* buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* MSGBUS_PROTOBUF_H */
