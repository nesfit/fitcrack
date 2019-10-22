<template>
  <grid-tile
    :icon="jobIcon(item.status)"
    :color="jobColor(item.status)"
  >
    <span>
      <b>{{ item.count }}</b> {{ item.status }}
    </span>
  </grid-tile>
</template>

<script>
import gridTile from './gridTile'
export default {
  components: {
    gridTile
  },
  props: {
    item: {
      type: Object,
      default: () => ({})
    }
  },
  methods: {
    jobColor (status) {
      const colors = {
        ready: 'info',
        finished: 'success',
        exhausted: 'warning',
        malformed: 'error',
        finishing: ''
      }
      return colors[status]
    },
    jobIcon (status) {
      const icons = {
        ready: 'mdi-play-circle-outline',
        finished: 'mdi-checkbox-marked-circle-outline',
        exhausted: 'mdi-close-circle-outline',
        malformed: 'mdi-alert-circle-outline',
        finishing: 'mdi-progress-clock'
      }
      return icons[status]
    }
  }
}
</script>