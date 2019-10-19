<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-progress-circular
      v-if="notifications === null"
      indeterminate
      :size="50"
      color="primary"
      class="mx-auto mt-3 d-block"
    />
    <div v-else>
      <v-list
        v-for="(item, i) in notifications.items"
        :key="i"
        class="notifContainer pa-0"
      >
        <notification
          :type="item.type"
          :text="item.text"
          :seen="item.seen"
          :time="item.time"
          :job-id="item.job_id"
        />
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
    data: function () {
      return {
        notifications: null
      }
    },
    methods: {
      loadNotifications: function () {
        this.notifications = null;
        this.axios.get(this.$serverAddr + '/notifications').then((response) => {
          this.notifications = response.data
        })
      }
    }
  }
</script>
<style scoped>

</style>
