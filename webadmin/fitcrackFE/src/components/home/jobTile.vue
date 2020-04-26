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
import { jobIcon } from '@/assets/scripts/iconMaps'
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
        exhausted: 'error',
        malformed: 'error',
        finishing: 'info'
      }
      return colors[status]
    },
    jobIcon
  }
}
</script>