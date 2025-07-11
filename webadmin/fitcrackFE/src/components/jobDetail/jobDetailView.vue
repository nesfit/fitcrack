<template>
  <div class="jd-container">
    <v-progress-linear
      v-if="!data"
      indeterminate
      color="secondary"
    />
    <v-sheet class="jd-split">
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
            v-model="openPanels"
            accordion
            multiple
            flat
            hover
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
                        v-if="(data.permissions.operate || $userCan('OPERATE_ALL_JOBS')) && parseInt(data.status) >= 10"
                        :color="purging ? '' : 'error'"
                        :ripple="false"
                        @click="purgeHandler"
                      >
                        <span>{{ purging ? 'Click to confirm' : 'Kill' }}</span>
                        <v-icon right>
                          mdi-cog-stop
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
                  <v-col>
                    <component
                      :is="attackDetailComponent"
                      :data="data"
                      class="attack-info"
                    />
                  </v-col>
                  <v-col>
                    <hash-table
                      class="grow"
                      link-back
                      :id="data.hash_list_id"
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
                      show-chart-patterns
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
                    <job-contribution-chart :id="data.id" />
                  </v-col>
                </v-row>
              </v-expansion-panel-content>
            </v-expansion-panel>
            <v-expansion-panel @change="statsToggled">
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
                <v-row ref="chartrow">
                  <!-- sorry mobile, i have had enough of this -->
                  <v-col cols="6">
                    <v-card-title>Progress over time</v-card-title>
                    <job-progress-chart :id="data.id" />
                  </v-col>
                  <v-col cols="6">
                    <v-card-title>Hashes in workunits</v-card-title>
                    <job-workunit-chart :id="data.id" />
                  </v-col>
                </v-row>
              </v-expansion-panel-content>
            </v-expansion-panel>
          </v-expansion-panels>
        </div>
      </transition>
    </v-sheet>
    <v-divider />
    <workunits
      v-if="data"
      show-chart-patterns
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
import jobInfo from './jobInfo.vue'
import workunits from './workunits.vue'
import statusTimeline from './statusTimeline.vue'
import hostTable from './hostTable.vue'
import hostEditor from './hostEditor.vue'
import permsEditor from './jobPermissions.vue'
//
import hashTable from '@/components/hashlist/hashTable.vue'
//
import jobProgressChart from '@/components/chart/jobProgress.vue'
import jobWorkunitChart from '@/components/chart/jobWorkunits.vue'
import jobContributionChart from '@/components/chart/jobContribution.vue'
//
import combinatorDetail from '@/components/jobDetail/attacks/combinator.vue'
import hybridDetail from '@/components/jobDetail/attacks/hybrid.vue'
import maskDetail from '@/components/jobDetail/attacks/mask.vue'
import dictionaryDetail from '@/components/jobDetail/attacks/dictionary.vue'
import princeDetail from '@/components/jobDetail/attacks/prince.vue'
import pcfgDetail from '@/components/jobDetail/attacks/pcfg.vue'
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
    jobProgressChart,
    jobWorkunitChart,
    jobContributionChart,
    combinatorDetail,
    hybridDetail,
    maskDetail,
    dictionaryDetail,
    princeDetail,
    pcfgDetail
  },
  data () {
    return {
      data: null,
      hashes: null,
      statusHistory: null,
      progressGraph: null,
      hostGraph: null,
      hostPercentageGraph: null,
      //
      hostEditorOpen: false,
      permsEditorOpen: false,
      openPanels: [0,1,2],
      purging: false,
      //
      chartHeight: '0px'
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
        case 'combinator':
          return 'combinatorDetail'
        default:
          return 'hybridDetail'
      }
    },
    shouldConfirmPurge () {
        if (!localStorage.hasOwnProperty('confirmpurge'))
          return true
        return localStorage.getItem('confirmpurge') == 'true'
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
      // Merged masks should not be visible
      this.data.masks = this.data.masks.filter(item => item.merged == 0)
      document.title = `${this.data.name} – ${this.$store.state.project}`
    },
    loadAll () {
      this.loadData()
      this.loadHistory()
    },
    //
    operateJob (operation) {
      this.axios.get(this.$serverAddr + '/job/' + this.data.id + '/action', {
        params: {
          'operation': operation
        }
      }).then(this.loadAll)
    },
    purgeHandler () {
      if (this.shouldConfirmPurge && !this.purging) {
        this.purging = true
        setTimeout(() => {
          this.purging = false
        }, 3000)
      } else {
        this.purging = false
        this.operateJob('kill')
      }
    },
    statsToggled () {
      // hack to restore chart height after closing the panel
      const wrappers = this.$refs.chartrow.querySelectorAll('.chart-wrapper')
      Array.from(wrappers).forEach(w => {
        const current = getComputedStyle(w).getPropertyValue('height')
        if (current !== '0px' && current !== 'auto') {
          this.chartHeight = current
        } else {
          w.style.height = this.chartHeight
        }
      })
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