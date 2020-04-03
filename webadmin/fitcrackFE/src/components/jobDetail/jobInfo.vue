<template>
  <div class="job-info">
    <!-- TOP SHEET -->
    <v-sheet
      class="top-sheet pa-4"
      dark
      tile
      :color="data.status_type"
    >
      <v-icon
        class="attack-icon"
        aria-hidden
      >
        {{ attackIcon(data.attack) }}
      </v-icon>
      <div class="top-sheet-bar mb-2">
        <div>
          <div class="name title mb-2">
            {{ data.name }}
          </div>
          <div class="subtitle-2 ">
            <b class="text-capitalize">{{ data.attack }}</b> Attack
            <br>
            <b>{{ data.hash_type_name }}</b>
          </div>
        </div>
        <v-tooltip left>
          <template #activator="{ on }">
            <v-btn
              icon
              v-on="on"
            >
              <v-icon>mdi-settings</v-icon>
            </v-btn>
          </template>
          <span>Options</span>
        </v-tooltip>
      </div>
      <div class="status">
        <v-progress-circular
          class="fu"
          :value="data.progress"
          size="26"
          rotate="-90"
        />
        <span class="text-capitalize pt-1 ml-2">
          <b v-show="data.progress > 0 && data.progress < 100">{{ data.progress }} %</b>
          {{ data.status_text }}
        </span>
      </div>
      <!-- CTA FAB -->
      <v-tooltip left>
        <template #activator="{ on }">
          <v-btn
            :disabled="data.host_count == 0"
            fab
            small
            absolute
            bottom
            right
            light
            class="z"
            @click="operateJob(data.id, operation.text)"
            v-on="on"
          >
            <v-icon>
              {{ operation.icon }}
            </v-icon>
          </v-btn>
        </template>
        <span class="text-capitalize">{{ operation.text }}</span>
      </v-tooltip>
    </v-sheet>
    <!-- INFO LIST -->
    <v-list 
      class="info-list"
      inactive
      two-line
    >
      <v-list-item
        v-for="({title, icon, value}, i) in details"
        :key="i"
      >
        <v-list-item-content>
          <v-list-item-subtitle class="overline mb-1">
            <v-icon
              class="mr-1"
              small
            >
              {{ icon }}
            </v-icon>
            {{ title }}
          </v-list-item-subtitle>
          <v-list-item-title class="wrap">
            {{ value }}
          </v-list-item-title>
        </v-list-item-content>
      </v-list-item>
    </v-list>
  </div>
</template>

<script>
import { jobIcon, attackIcon } from '@/assets/scripts/iconMaps'
import fmt from '@/assets/scripts/numberFormat'

export default {
  props: {
    data: {
      type: Object,
      default: () => {}
    }
  },
  computed: {
    operation () {
      if (this.data.status === '0') {
        return {text: 'start', icon: 'mdi-play'}
      } else if (this.data.status < 10) {
        return {text: 'restart', icon: 'mdi-restart'}
      } else {
        return {text: 'stop', icon: 'mdi-stop'}
      }
    },
    // Dynamically create the info list items
    details () {
      const d = this.data
      const items = [
        {title: 'Comment', icon: 'mdi-format-quote-close', value: d.comment},
        {title: 'Keyspace', icon: 'mdi-key', value: d.keyspace, format: fmt},
        {title: 'Date Added', icon: 'mdi-calendar-import', value: d.time, format: v => this.$moment(v).calendar()},
        {title: 'WU Sum Time', icon: 'mdi-sigma', value: d.cracking_time_str},
        {title: 'Cracking Time', icon: 'mdi-timer', value: this.crackingTime},
        {title: 'Start', icon: 'mdi-ray-start', value: this.startTime},
        {title: 'End', icon: 'mdi-ray-end', value: this.endTime},
        {title: 'Efficiency', icon: 'mdi-chart-timeline-variant', value: this.efficiency},
        {title: 'Time per WU', icon: 'mdi-timeline-clock', value: d.seconds_per_job, format: v => `${v} seconds`},
      ]
      // Leave out missing stuff and format fields
      return items.filter(i => i.value != null && i.value != undefined & i.value !== '').map(i => {
        if (i.format) {
          i.value = i.format(i.value)
        }
        return i
      })
    },
    crackingTime () {
      const d = this.data
      if (d.time_end) {
        return this.$moment.utc(this.$moment(d.time_end).diff(this.$moment(d.time_start))).format("HH:mm:ss")
      } else if (d.time_start) {
        return this.$moment.utc(this.$moment().diff(this.$moment(d.time_start))).format("HH:mm:ss")
      } else {
        return 'Not started yet'
      }
    },
    startTime () {
      const d = this.data
      if (d.time_start) {
        return this.$moment(d.time_start).format('DD.MM.YYYY HH:mm')
      } else {
        return 'Not started yet'
      }
    },
    endTime () {
      const d = this.data
      if (d.time_end) {
        return this.$moment(d.time_end).format('DD.MM.YYYY HH:mm')
      } else {
        return 'Not planned'
      }
    },
    efficiency () {
      const validWorkunits = this.data.workunits.filter(workunit => workunit.hc_keyspace !== 0)
      if (this.data.hosts.length > 0 && validWorkunits.length > 0 && this.data.cracking_time > 0) {
        const efficiency = (validWorkunits.map(workunit => workunit.cracking_time).reduce((a, b) => a + b)) / (this.data.hosts.length * this.data.cracking_time)
        return (efficiency * 100).toFixed(2) + ' %'
      } else {
        return null
      }
    }
  },
  methods: {
    jobIcon,
    attackIcon,
    fmt
  }
}
</script>

<style scoped>
.job-info {
  display: flex;
  flex-direction: column;
}

.top-sheet {
  position: relative;
}

.info-list {
  flex-grow: 1;
}

.wrap { white-space: normal; }

.top-sheet-bar {
  display: flex;
  justify-content: space-between;
}

.status {
  display: inline-flex;
  align-items: center;
}

/**/

.attack-icon {
  position: absolute;
  font-size: 8em;
  z-index: 0;
  opacity: .3;
  right: -.23em;
  top: .1em;
}

.z { z-index: 1 }

</style>

<style>
.v-progress-circular__underlay {
  stroke: rgba(255,255,255,.3);
}
</style>