<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <div class="cont">
      <v-list-tile :to="'/jobs/' + jobId">
        <v-alert :type="type" value="true" class="ma-0 pa-0 width100" id="notif" v-bind:class="{ seen: seen}" outline>
          <v-layout row wrap class="width100">
            <v-flex class="text-xs-left">{{text}}</v-flex>
            <v-spacer></v-spacer>
            <v-flex class="text-xs-right">{{$moment(time).format('DD.MM.YYYY HH:mm')}}</v-flex>
          </v-layout>
        </v-alert>
      </v-list-tile>
      <v-divider></v-divider>
    </div>
</template>

<script>
    export default {
      name: 'notification',
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
