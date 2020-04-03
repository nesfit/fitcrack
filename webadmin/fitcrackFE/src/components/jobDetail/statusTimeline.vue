<template>
  <v-timeline dense>
    <v-timeline-item
      v-for="{ time, status_text, status_type } in entries"
      :key="time"
      class="invisible"
      :color="''/* NOPE */"
      fill-dot
    >
      <template #icon>
        <v-icon :color="status_type">
          {{ jobIcon(status_text) }}
        </v-icon>
      </template>
      <div class="overline">
        {{ status_text }}
      </div>
      <div class="subtitle-2">
        {{ $moment(time).format('DD.MM.YYYY') }}
      </div>
      <div>
        {{ $moment(time).format('HH:mm') }}
      </div>
    </v-timeline-item>
  </v-timeline>
</template>

<script>
import { jobIcon } from '@/assets/scripts/iconMaps'

export default {
  props: {
    entries: {
      type: Array,
      default: () => []
    }
  },
  methods: {
    jobIcon
  }
}
</script>

<style>
  .invisible .v-timeline-item__dot {
    box-shadow: none;
  }
</style>