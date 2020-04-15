<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-list-item :to="'/jobs/' + jobId">
    <v-alert
      class="d-inline-block width100 my-2"
      :type="type"
      :text="!seen"
      outlined
    >
      <div class="text-left">
        {{ 'Job \"' + title + '\"' }}
      </div>
      <v-divider />
      <div class="text-left">
        {{ text }}
      </div>
      <small>
        {{ $moment(time).format('DD. MM. YYYY HH:mm') }}
      </small>
    </v-alert>
  </v-list-item>
</template>

<script>
    export default {
      name: 'Notification',
      props: ['type', 'title', 'text', 'seen', 'time', 'jobId']
    }
</script>

<style scoped>

</style>
