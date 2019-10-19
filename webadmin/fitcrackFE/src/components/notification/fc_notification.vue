<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="cont">
    <v-list-item :to="'/jobs/' + jobId">
      <v-alert
        id="notif"
        :type="type"
        class="ma-0 pa-0 width100"
        :class="{ seen: seen}"
        outlined
      >
        <v-row class="width100">
          <v-col class="text-left">
            {{ text }}
          </v-col>
          <v-spacer />
          <v-col class="text-right">
            {{ $moment(time).format('DD.MM.YYYY HH:mm') }}
          </v-col>
        </v-row>
      </v-alert>
    </v-list-item>
    <v-divider />
  </div>
</template>

<script>
    export default {
      name: 'Notification',
      props: ['type', 'text', 'seen', 'time', 'jobId']
    }
</script>

<style scoped>
  .cont{
    display: block;
    width: 100%;
  }

  #notif {
    border: none !important;
  }

  .width100 {
    width: 100%;
  }

  .seen {
    opacity: 0.5;
  }
</style>
