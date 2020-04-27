<template>
  <div class="jd-container">
    <v-progress-linear
      v-if="!data"
      indeterminate
      color="secondary"
    />
    <div class="jd-split">
      <transition name="info">
        <job-info
          v-if="data"
          :data="data"
          class="jd-info"
          @operate="operateJob"
          @edit-hosts="hostEditorOpen = true"
          @edit-permissions="permsEditorOpen = true"
          @reload="loadData"
        />
      </transition>
      <v-divider vertical />
      <transition name="main">
        <div
          v-if="data"
          class="jd-main"
        >
          <v-expansion-panels 
            accordion
            multiple
            flat
            hover
            :value="openPanels"
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
                <v-row v-if="statusHistory && statusHistory.length > 1">
                  <v-col>
                    <v-card-title>
                      <span>Status History</span>
                      <v-spacer />
                      <v-btn
                        v-if="data.permissions.operate || $userCan('OPERATE_ALL_JOBS')"
                        color="error"
                        @click="operateJob('kill')"
                      >
                        <span>Purge</span>
                        <v-icon right>
                          mdi-undo-variant
                        </v-icon>
                      </v-btn>
                    </v-card-title>
                    <status-timeline 
                      class="history px-4"
                      :entries="statusHistory"
                    />
                  </v-col>
                </v-row>
                <v-row>
                  <v-col md="4">
                    <component
                      :is="attackDetailComponent"
                      :data="data"
                      class="attack-info"
                    />
                  </v-col>
                  <v-col md="8">
                    <v-card flat>
                      <v-card-title>Hashes</v-card-title>
                      <v-card-text>
                        <hash-table
                          class="grow"
                          :hashes="data.hashes"
                        />
                      </v-card-text>
                    </v-card>
                  </v-col>
                </v-row>
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
                <v-row class="align-center">
                  <v-col cols="8">
                    <host-table
                      :hosts="data.hosts"
                    />
                    <div class="d-flex mt-2">
                      <v-spacer />
                      <v-btn
                        v-if="data.permissions.edit || $userCan('EDIT_ALL_JOBS')"
                        color="info"
                        @click="hostEditorOpen = true"
                      >
                        <span>Assign Hosts</span>
                        <v-icon right>
                          mdi-playlist-edit
                        </v-icon>
                      </v-btn>
                    </div>
                  </v-col>
                  <v-col cols="4">
                    <graph
                      id="hostPercentageGraph"
                      :data="hostPercentageGraph"
                    />
                  </v-col>
                </v-row>
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
                <v-row>
                  <!-- sorry mobile, i have had enough of this -->
                  <v-col cols="6">
                    <v-card-title>Progress over time</v-card-title>
                    <graph
                      id="progressGraph"
                      :data="progressGraph"
                      units="%"
                      type="job"
                    />
                  </v-col>
                  <v-col cols="6">
                    <v-card-title>Hashes in workunits</v-card-title>
                    <graph
                      id="hostGraph"
                      :data="hostGraph"
                      units=" hashes"
                      type="host"
                    />
                  </v-col>
                </v-row>
              </v-expansion-panel-content>
            </v-expansion-panel>
          </v-expansion-panels>
        </div>
      </transition>
    </div>
    <v-divider />
    <workunits
      v-if="data"
      class="wu-container"
      :data="data"
    />
    <!-- Dialogs -->
    <host-editor
      v-if="data"
      v-model="hostEditorOpen"
      :job-ids="[data.id]"
      @reload="loadData"
    />
    <perms-editor
      v-if="data"
      v-model="permsEditorOpen"
      :job-id="data.id"
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
import hostEditor from './hostEditor'
import permsEditor from './jobPermissions'
//
import graph from '@/components/graph/fc_graph'
import combinatorDetail from '@/components/job/attacksDetail/combinator'
import maskDetail from '@/components/job/attacksDetail/mask'
import dictionaryDetail from '@/components/job/attacksDetail/dictionary'
import princeDetail from '@/components/job/attacksDetail/prince'
import pcfgDetail from '@/components/job/attacksDetail/pcfg'
// Scripts
import { jobIcon, attackIcon } from '@/assets/scripts/iconMaps'
//
let interval
//
export default {
  components: {
    jobInfo,
    workunits,
    statusTimeline,
    hashTable,
    hostTable,
    hostEditor,
    permsEditor,
    graph,
    combinatorDetail,
    maskDetail,
    dictionaryDetail,
    princeDetail,
    pcfgDetail
  },
  data () {
    return {
      data: null,
      statusHistory: null,
      progressGraph: null,
      hostGraph: null,
      hostPercentageGraph: null,
      //
      hostEditorOpen: false,
      permsEditorOpen: false,
      openPanels: [0,1,2]
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
        case 'prince':
          return 'princeDetail'
        default:
          return 'combinatorDetail'
      }
    }
  },
  mounted () {
    this.loadAll()
    interval = setInterval(this.loadAll, 5000)
  },
  beforeDestroy () {
    clearInterval(interval)
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
      document.title = `${this.data.name} – ${this.$store.state.project}`
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
    },
    loadAll () {
      this.loadData()
      this.loadHistory()
      this.loadGraphs()
    },
    //
    operateJob (operation) {
      this.axios.get(this.$serverAddr + '/job/' + this.data.id + '/action', {
        params: {
          'operation': operation
        }
      }).then(this.loadAll)
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

  .info-enter-active {
    transition: all .7s cubic-bezier(0.16, 1, 0.3, 1);
  }
  .info-enter {
    transform: translateY(-15px);
    opacity: 0;
  }

  .main-enter-active {
    transition: all .7s;
  }
  .main-enter {
    opacity: 0;
  }

</style>