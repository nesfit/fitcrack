<template>
  <div class="container">
    <v-progress-linear
      v-show="loading"
      indeterminate
      absolute
    />
    <timeseries
      v-if="loaded"
      :chart-data="chartdata"
      :overrides="options"
    />
  </div>
</template>

<script>
import Timeseries from './types/timeseries'
import { prepareLines } from './helpers'
import autoload from './autoupdateMixin'
import watcher from './boundWatcherMixin'

export default {
  components: { Timeseries },
  mixins: [autoload, watcher],
  props: {
    id: {
      type: Number,
      default: undefined
    },
    from: {
      type: String,
      default: undefined
    },
    to: {
      type: String,
      default: undefined
    },
    batch: Boolean,
    logarithmic: Boolean
  },
  data: () => ({
    loaded: false,
    timeout: null,
    loading: false,
    chartdata: null,
  }),
  computed: {
    options () {
      return {
        legend: {
          display: false
        },
        scales: {
          y: {
            type: this.batch || this.logarithmic ? 'logarithmic' : 'linear',
            ticks: {
              autoSkipPadding: 15
            }
          }
        }
      }
    },
    fromTime () {
      if (!this.id && !this.from) {
        return this.$moment().subtract(24, 'hours').format('YYYY-M-DTH:mm:ss')
      } else {
        return this.from
      }
    }
  },
  methods: {
    loadData (manual = false) {
      if (manual) {
        this.loading = true
      }
      const endpoint = this.batch ? 'batch' : 'job'
      let target = `${this.$serverAddr}/chart/${endpoint}Workunits`
      if (this.id) target += `/${this.id}`
      this.axios.get(target, {
        params: {
          from: this.fromTime,
          to: this.to
        }
      }).then(r => {
        this.chartdata = {
          datasets: prepareLines(r.data.datasets)
        }
        this.loading = false
        this.loaded = true
      })
    }
  }
}
</script>

<style scoped>
.container {
  position: relative;
}
</style>