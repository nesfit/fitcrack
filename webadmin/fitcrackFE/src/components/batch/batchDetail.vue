<template>
  <div>
    <v-toolbar flat>
      <v-chip>
        <v-fade-transition mode="out-in">
          <v-progress-circular
            v-if="loading"
            class="back-margin"
            size="16"
            width="2"
            indeterminate
          />
          <span v-else>
            {{ data.total_jobs }}
          </span>
        </v-fade-transition>
      </v-chip>
      <h2 class="ml-4">
        {{ data ? data.name : '...' }}
      </h2>
      <v-spacer />
      <div>
        <transition 
          name="fade"
          mode="out-in"
        >
          <div 
            v-if="editing" 
            key="renameEditPanel"
            class="d-flex align-center"
          >
            <v-text-field
              v-model="data.name"
              hide-details
              class="mr-2"
            />
            <v-btn
              text
              :loading="loading"
              @click="saveEdit"
            >
              <v-icon left>
                mdi-content-save
              </v-icon>
              Save
            </v-btn>
            <v-btn
              text
              :ripple="false"
              @click="undoEdit"
            >
              <v-icon left>
                mdi-undo-variant
              </v-icon>
              Undo changes
            </v-btn>
          </div>
          <div 
            v-else
            key="batchActionsPanel"
          >
            <v-btn
              text
              :ripple="false"
              @click="editing = true"
            >
              <v-icon left>
                mdi-pencil
              </v-icon>
              Edit Batch
            </v-btn>
            <v-btn
              text
              @click="deleteBatchConfirm"
            >
              <v-icon left>
                mdi-link-variant-off
              </v-icon>
              Unlink Batch
            </v-btn>
          </div>
        </transition>
      </div>
    </v-toolbar>
    <div
      v-if="data"
      class="container"
    >
      <v-card class="mb-6">
        <v-card-title>
          <v-icon left>
            {{ data.status.icon }}
          </v-icon>
          <span>{{ data.status.text }}</span>
          <v-spacer />
          <span v-if="data.waiting_jobs > 0">
            {{ data.waiting_jobs }} queued to run
          </span>
        </v-card-title>
        <v-card-text>
          <v-progress-linear
            :value="totalProgress"
            :buffer-value="totalBuffer"
            stream
            rounded
          />
          <v-row>
            <v-col class="text-end">
              {{ totalProgress }} %
            </v-col>
          </v-row>
        </v-card-text>
        <v-card-actions v-if="data.waiting_jobs > 0">
          <!-- opration buttons -->
          <v-btn
            v-if="data.status.code < 10"
            color="success"
            @click="runBatch"
          >
            Run
            <v-icon right>
              mdi-play
            </v-icon>
          </v-btn>
          <v-btn
            v-else-if="data.status.code < 20"
            color="error"
            @click="interruptBatch"
          >
            Interrupt
            <v-icon right>
              mdi-pause
            </v-icon>
          </v-btn>
          <v-btn
            v-else
            color="error"
            @click="runBatch"
          >
            Resume
            <v-icon right>
              mdi-skip-next
            </v-icon>
          </v-btn>
        </v-card-actions>
      </v-card>
      <v-alert 
        v-show="editing"
        type="info"
      >
        Drag jobs to change run order
      </v-alert>
      <draggable
        v-model="data.jobs"
        :animation="150"
        :disabled="!editing"
        @change="sorting = undefined"
      >
        <v-list-item
          v-for="{ id, name, attack, progress, status_text, status_type } in data.jobs"
          :key="id"
        >
          <v-list-item-action>
            <v-icon :color="status_type">
              {{ jobIcon(status_text) }}
            </v-icon>
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title class="font-weight-bold">
              <router-link :to="{ name: 'jobDetail', params: { id } }">
                {{ name }}
              </router-link>
              <v-progress-linear
                :value="progress"
                rounded
                height="2"
                class="my-1"
              />
            </v-list-item-title>
            <v-list-item-subtitle>
              <span v-show="progress > 0 && progress < 100">{{ progress }} %</span> <span class="text-capitalize">{{ status_text }}</span>
              <span>{{ attack }} attack</span>
            </v-list-item-subtitle>
          </v-list-item-content>
          <v-list-item-action v-if="editing">
            <v-btn
              text
              @click="unqueueJob(id)"
            >
              <span>Unqueue</span>
              <v-icon right>
                mdi-tray-remove
              </v-icon>
            </v-btn>
          </v-list-item-action>
        </v-list-item>
      </draggable>
      <v-card class="mt-6">
        <v-card-text>
          <v-row>
            <v-col cols="3">
              <v-card-title>
                Host contribution
              </v-card-title>
              <graph
                id="hostPercentageGraph"
                :data="hostPercentageGraph"
              />
              <div class="mt-4 caption text-justify">
                For each host that contributed, this is a sum of all hashes from all job's workunits in this batch.
                For more info on distribution and propgress, see detailed view for each job.
              </div>
            </v-col>
            <v-col cols="9">
              <v-card-title>
                Hashes in workunits over time
              </v-card-title>
              <graph
                id="hostGraph"
                :data="hostGraph"
                units=" hashes"
                type="host"
              />
            </v-col>
          </v-row>
        </v-card-text>
      </v-card>
    </div>
  </div>
</template>

<script>
import { attackIcon, jobIcon } from '@/assets/scripts/iconMaps'

import graph from '@/components/graph/fc_graph'
import draggable from 'vuedraggable'

export default {
  components: {
    draggable,
    graph
  },
  data () {
    return {
      interval: null,
      data: null,
      original: null,
      editing: false,
      loading: true,
      //
      hostPercentageGraph: null,
      hostGraph: null
    }
  },
  computed: {
    totalProgress () {
      if (!this.data) return 0
      const total = this.data.jobs.reduce((prev, curr) => prev += curr.progress, 0)
      return Math.round(100 / (100 * this.data.total_jobs) * total)
    },
    totalBuffer () {
      if (!this.data) return 0
      return 100 / this.data.total_jobs * (this.data.total_jobs - this.data.waiting_jobs)
    }
  },
  mounted () {
    this.load()
    this.interval = setInterval(() => {
      if (!this.editing) {
        this.load()
      }
    }, 5000)
  },
  beforeDestroy: function () {
    clearInterval(this.interval)
  },
  methods: {
    attackIcon,
    jobIcon,
    async load () {
      this.loading = true
      this.loadGraphs()
      this.data = await this.axios.get(`${this.$serverAddr}/batches/${this.$route.params.id}`).then(r => r.data)
      this.original = {...this.data}
      this.loading = false
    },
    async loadGraphs () {
      const id = this.$route.params.id
      const graphPromises = [
        this.axios.get(`${this.$serverAddr}/graph/hostPercentage/batch/${id}`).then(r => r.data),
        this.axios.get(`${this.$serverAddr}/graph/hostsComputing/batch/${id}`).then(r => r.data)
      ]
      try {
        [this.hostPercentageGraph, this.hostGraph] = await Promise.all(graphPromises)
      } catch (e) {
        console.error('Error getting batch graphs', e)
      }
    },
    deleteBatchConfirm () {
      this.$root.$confirm('Unlink Batch', `This will unlink jobs from this batch and remove the batch. Jobs will not be discarded. If a job is currently running, it will finish normally. Are you sure?`)
      .then(async confirm => {
        try {
          await this.axios.delete(`${this.$serverAddr}/batches/${this.data.id}`)
          this.$router.push('/batches')
        } catch (e) {
          console.error(e)
        }
      })
    },
    async saveEdit () {
      this.loading = true
      try {
        await this.axios.put(`${this.$serverAddr}/batches`, {
          id: this.data.id,
          name: this.data.name,
          jobs: this.data.jobs.map(j => j.id)
        })
      } catch (e) {
        console.error(e)
      } finally {
        this.editing = false
        this.load()
      }
    },
    undoEdit () {
      this.editing = false
      this.data = {...this.original}
    },
    unqueueJob (id) {
      this.data.jobs = this.data.jobs.filter(job => job.id != id)
    },
    async runBatch () {
      try {
        await this.axios.post(`${this.$serverAddr}/batches/${this.data.id}/run`)
        this.load()
      } catch (e) {
        console.error(e)
      }
    },
    async interruptBatch () {
      try {
        await this.axios.post(`${this.$serverAddr}/batches/${this.data.id}/interrupt`)
        this.load()
      } catch (e) {
        console.error(e)
      }
    }
  }
}
</script>

<style scoped>
  .fade-enter-active, .fade-leave-active {
    transition: opacity .1s, transform .2s cubic-bezier(0.16, 1, 0.3, 1);
  }

  .fade-leave-to {
    opacity: 0;
  }
  .fade-enter {
    transform: translateX(10%)
  }
</style>