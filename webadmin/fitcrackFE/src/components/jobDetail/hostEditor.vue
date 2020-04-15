<template>
  <v-bottom-sheet 
    inset
    :value="value"
    @input="$emit('input')"
  >
    <v-card>
      <v-card-title>
        <span>Host Mapping</span>
        <v-spacer />
        <v-btn 
          text
          :loading="saving"
          color="success"
          @click="save"
        >
          <v-icon left>
            mdi-check
          </v-icon>
          Done
        </v-btn>
        <v-btn
          icon
          @click="$emit('input')"
        >
          <v-icon>mdi-close</v-icon>
        </v-btn>
      </v-card-title>
      <v-card-text>
        <host-selector
          v-model="hosts"
          select-all
          auto-refresh
        />
        <v-row>
          <v-col>
            <v-btn
              text
              to="/hosts"
            >
              <v-icon left>
                mdi-desktop-classic
              </v-icon>
              Host List
            </v-btn>
          </v-col>
        </v-row>
      </v-card-text>
    </v-card>
  </v-bottom-sheet>
</template>

<script>
import hostSelector from '@/components/selector/hostSelector'

export default {
  components: {
    hostSelector
  },
  props: {
    value: Boolean,
    jobIds: {
      type: Array,
      required: true
    }
  },
  data () {
    return {
      hosts: [],
      saving: false
    }
  },
  methods: {
    async save () {
      this.saving = true
      try {
        await this.axios.post(this.$serverAddr + '/job/host', {
          'job_ids': this.jobIds,
          'newHost_ids': this.hosts.map(host => host.id)
        })
        this.$emit('input') // close
        this.$emit('reload')
      } catch (e) {
        console.error(e)
      } finally {
        this.saving = false
      }
    }
  }
}
</script>