#include <concord/discord.h>
#include <concord/log.h>
#include <string.h>

void on_ready(struct discord *client, const struct discord_ready *event) {
  log_info("Logged in as %s!", event->user->username);
}

void on_message(struct discord *client, const struct discord_message *event) {
  if (event->author->bot)
    return;

  if (strcmp(event->content, "ping") == 0) {
    struct discord_create_message params = {.content = "pong"};
    discord_create_message(client, event->channel_id, &params, NULL);
  }

  if (strcmp(event->content, "embed") == 0) {
    struct discord_embed_field fields[] = {
        {
            .name = "Field 1",
            .value = "Value 1",
            .Inline = false,
        },
        {
            .name = "Field 2",
            .value = "Value 2",
            .Inline = false,
        },
    };

    char avatar_url[256];
    snprintf(avatar_url, sizeof(avatar_url), "https://cdn.discordapp.com/avatars/%" PRIu64 "/%s.png",
             event->author->id, event->author->avatar);

    struct discord_embed embeds[] =
    { {
        .title = "Test embed",
        .description = "This is a test embed",
        .color = 0x00FF00,
        .thumbnail =
            &(struct discord_embed_thumbnail){.url = avatar_url },
        .footer = &(struct discord_embed_footer){.text = "Test footer"},
        .fields =
            &(struct discord_embed_fields){
                .size = 2,
                .array = fields,
            },
    }, };

    struct discord_create_message params = {
        .embeds =
            &(struct discord_embeds){
                .size = 1,
                .array = embeds,
            },
    };
    discord_create_message(client, event->channel_id, &params, NULL);
  }
}

int main(void) {
  ccord_global_init();
  struct discord *client = discord_config_init("./config.json");
  discord_add_intents(client, DISCORD_GATEWAY_MESSAGE_CONTENT);
  discord_set_on_ready(client, &on_ready);
  discord_set_on_message_create(client, &on_message);
  discord_run(client);
  discord_cleanup(client);
  ccord_global_cleanup();
}