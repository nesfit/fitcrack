<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <div>
      <v-progress-circular indeterminate :size="50" color="primary" v-if="notifications === null" class="mx-auto mt-3 d-block"></v-progress-circular>
      <div v-else>
        <v-list class="notifContainer pa-0"
                v-for="(item, i) in notifications.items"
                :key="i"
        >
          <notification :type="item.type"
                        :text="item.text"
                        :seen="item.seen"
                        :time="item.time"
                        :job-id="item.job_id"
          ></notification>
        </v-list>
      </div>
    </div>
</template>

<script>
  import notification from '@/components/notification/fc_notification'

  export default {
    components: {
      'notification': notification
    },
    methods: {
      loadNotifications: function () {
        this.notifications = null;
        this.axios.get(this.$serverAddr + '/notifications').then((response) => {
          this.notifications = response.data
        })
      }
    },
    data: function () {
      return {
        notifications: null
      }
    }
  }
</script>
<style scoped>

</style>
