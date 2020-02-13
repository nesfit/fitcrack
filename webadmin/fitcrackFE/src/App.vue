<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-app>
    <v-snackbar
      v-model="alert"
      :timeout="6000"
      bottom
      right
      color="transparent"
      class="errorSnackbar pa-0"
    >
      <v-alert
        v-model="alert"
        :type="alertType"
        class="height100 ma-0 width100"
        :value="true"
        dismissible
        transition="none"
      >
        {{ alertText }}
      </v-alert>
    </v-snackbar>
    <router-view 
      :is-dark="isDark" 
      @alert="setAlert"
    />
    <vue-progress-bar />
    <confirm ref="confirm" />
  </v-app>
</template>

<script>
  import confirm from "@/components/confirmDialog/confirm";
  export default {
    name: 'App',
    components: {
      'confirm': confirm
    },
    data: function () {
      return {
        alert: false,
        alertText: '',
        alertType: 'error',
        isDark: false
      }
    },
    created: function () {
      /*
      this.$store.state.loggedInLink = this.$route;
      if (this.$needLogin) {
        this.$router.push({
          name: 'login'
        })
      }
      */
      this.axios.interceptors.response.use(
        function (response) {
          if (response.data.hasOwnProperty('status') && response.data.hasOwnProperty('message') && response.data.status) {
            this.alertType = 'success';
            this.alert = true;
            this.alertText = response.data.message
          }
          return response
        }.bind(this),
        function (error) {
          this.alertType = 'error';
          this.alert = true;
          if (error.response !== undefined) {
            if (error.response.data.message === 'Input payload validation failed') {
              let property = Object.keys(error.response.data.errors)[0]
              this.alertText = 'Wrong settings. ' +  property + ' (' + error.response.data.errors[property] + ')'
            }
            else {
              this.alertText = error.response.data.message
            }
          } else if (error.message !== undefined) {
            this.alertText = error.message
          } else {
            this.alertText = 'Can not connect to ' + this.$serverAddr
            //this.alertText = error.message
          }
        }.bind(this)
      )

      const mql = window.matchMedia('(prefers-color-scheme: dark)')
      mql.addListener(e => {
        this.$vuetify.theme.dark = e.matches
      })
      this.$vuetify.theme.dark = mql.matches
    },
    mounted () {
      this.$root.$confirm = this.$refs.confirm.open
    },
    methods: {
      setAlert ({type, text}) {
        this.alertType = type
        this.alertText = text
        this.alert = true
      }
    }
  }
</script>
<style scoped>

  .errorSnackbar {
    overflow: visible;
    padding: 0;
    z-index: 6;
  }

</style>
<style>
  h1, h2 {
    font-weight: normal;
  }

  .errorSnackbar .v-snack__content {
    padding: 0;
  }

  a {
    text-decoration: none;
  }

  .width100 {
    width: 100%;
  }


</style>

