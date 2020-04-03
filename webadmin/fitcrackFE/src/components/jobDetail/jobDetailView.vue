<template>
  <div class="jd-container">
    <div class="jd-split">
      <job-info
        :data="data"
        class="jd-info"
      />
      <v-divider vertical />
      <div class="jd-main">
        <v-expansion-panels 
          accordion
          multiple
          flat
          hover
          :value="[0,1]"
        >
          <v-expansion-panel>
            <v-expansion-panel-header>
              <span class="expansion-header">
                <v-icon left>
                  mdi-file-document-box-search-outline
                </v-icon>
                <b class="title">Details</b>
              </span>
            </v-expansion-panel-header>
            <v-expansion-panel-content>
              <div class="details-split">
                <!-- <component
                  :is="attackDetailComponent"
                  :data="data"
                  class="attack-info"
                /> -->
                <hash-table
                  class="grow"
                  :hashes="data.hashes"
                />
                <status-timeline 
                  class="history mr-8"
                  :entries="statusHistory"
                />
              </div>
            </v-expansion-panel-content>
          </v-expansion-panel>
          <v-expansion-panel>
            <v-divider />
            <v-expansion-panel-header>
              <span class="expansion-header">
                <v-icon left>
                  mdi-desktop-classic
                </v-icon>
                <b class="title">Hosts</b>
              </span>
            </v-expansion-panel-header>
            <v-expansion-panel-content>
              <host-table
                :hosts="data.hosts"
              />
            </v-expansion-panel-content>
          </v-expansion-panel>
          <v-expansion-panel>
            <v-divider />
            <v-expansion-panel-header>
              <span class="expansion-header">
                <v-icon left>
                  mdi-poll
                </v-icon>
                <b class="title">Statistics</b>
              </span>
            </v-expansion-panel-header>
            <v-expansion-panel-content>
              Bad graphs breaking layout (don't) go here
            </v-expansion-panel-content>
          </v-expansion-panel>
        </v-expansion-panels>
      </div>
    </div>
    <v-divider />
    <workunits
      class="wu-container"
      :data="data"
    />
  </div>
</template>

<script>
// Components
import jobInfo from './jobInfo'
import workunits from './workunits'
import statusTimeline from './statusTimeline'
import hashTable from './hashTable'
import hostTable from './hostTable'
//
import graph from '@/components/graph/fc_graph'
import combinatorDetail from '@/components/job/attacksDetail/combinator'
import maskDetail from '@/components/job/attacksDetail/mask'
import dictionaryDetail from '@/components/job/attacksDetail/dictionary'
import pcfgDetail from '@/components/job/attacksDetail/pcfg'
// Scripts
import { jobIcon, attackIcon } from '@/assets/scripts/iconMaps'
//
export default {
  components: {
    jobInfo,
    workunits,
    statusTimeline,
    hashTable,
    hostTable,
    graph,
    combinatorDetail,
    maskDetail,
    dictionaryDetail,
    pcfgDetail
  },
  data () {
    return {
      data: null,
      statusHistory: null,
      progressGraph: null,
      hostGraph: null,
      hostPercentageGraph: null
    }
  },
  computed: {
    attackDetailComponent () {
      switch (this.data.attack) {
        case 'mask':
          return 'maskDetail'
        case 'dictionary':
          return 'dictionaryDetail'
        case 'pcfg':
          return 'pcfgDetail'
        // dictionaryDetail is good enough for PRINCE
        case 'prince':
          return 'dictionaryDetail'
        default:
          return 'combinatorDetail'
      }
    }
  },
  mounted () {
    this.loadData()
    this.loadHistory()
    this.loadGraphs()
  },
  methods: {
    jobIcon,
    attackIcon,
    //
    async loadHistory () {
      this.statusHistory = await this.axios.get(this.$serverAddr + '/status/' + this.$route.params.id).then(r => r.data.items)
    },
    async loadData () {
      this.data = await this.axios.get(this.$serverAddr + '/job/' + this.$route.params.id).then(r => r.data)
    },
    async loadGraphs () {
      const id = this.$route.params.id
      const graphPromises = [
        this.axios.get(this.$serverAddr + '/graph/jobsProgress/'   + id).then(r => r.data),
        this.axios.get(this.$serverAddr + '/graph/hostsComputing/' + id).then(r => r.data),
        this.axios.get(this.$serverAddr + '/graph/hostPercentage/' + id).then(r => r.data)
      ]
      try {
        [this.progressGraph, this.hostGraph, this.hostPercentageGraph] = await Promise.all(graphPromises)
      } catch (e) {
        console.error('Error getting job graphs', e)
      }
    }
  }
}
</script>

<style scoped>
.jd-split {
  display: flex;
  flex-wrap: wrap;
}

.jd-info {
  flex: 1 0 350px;
}

.jd-main {
  flex: 9999 0 500px;
}

.wu-container {
  width: 100%;
}

.expansion-header {
  display: flex;
  align-items: center;
}

/* details */

.details-split {
  display: flex;
  flex-wrap: wrap;
  align-items: flex-start;
}

.details-split .grow {
  flex-grow: 1;
}

/**/

</style>