<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="height100">
    <v-navigation-drawer
      v-model="drawer"
      persistent
      class="navigationDrawer"
      enable-resize-watcher
      fixed
      app
      width="285"
    >
      <router-link :to="{ name: 'home'}">
        <img
          v-if="$vuetify.theme.dark"
          :src="require('@/assets/fitcrack-glow.svg')"
          class="mx-auto px-2 mt-2 d-block logo"
          alt="logo"
        >
        <img
          v-else
          :src="require('@/assets/fitcrack.svg')"
          class="mx-auto px-2 mt-2 d-block logo"
          alt="logo"
        >
      </router-link>

      <div class="dash-link">
        <v-btn
          to="/"
          text
        >
          <v-icon left>
            mdi-view-dashboard
          </v-icon>
          Dashboard
        </v-btn>
      </div>
        

      <v-divider />

      <v-tabs
        v-model="navtab"
        icons-and-text
        grow
      >
        <v-tab>
          Jobs
          <v-icon>mdi-briefcase-outline</v-icon>
        </v-tab>
        <v-tab>
          Library
          <v-icon>mdi-folder-outline</v-icon>
        </v-tab>
        <v-tab>
          System
          <v-icon>mdi-settings-outline</v-icon>
        </v-tab>
      </v-tabs>

      <v-tabs-items v-model="navtab">
        <v-tab-item>
          <v-list
            expand
            nav
            dense
          >
            <v-list-item
              v-if="$userCanAddJob()"
              :to="{ name: 'addJob'}"
            >
              <v-list-item-action>
                <v-icon>{{ routeIcon('addJob') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Add job</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <v-list-item 
              :to="{ name: 'jobs'}"
              exact
            >
              <v-list-item-action>
                <v-icon>{{ routeIcon('jobs') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>All jobs</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <v-divider class="mb-1" />

            <v-subheader>
              <b>JOB BINS</b>
              <v-spacer />
              <v-btn small text disabled>
                new
                <v-icon right>mdi-plus</v-icon>
              </v-btn>
            </v-subheader>

            <!-- ADD BIN
            <v-list-item>
              <v-list-item-action>
                <v-icon>mdi-folder</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-text-field hide-details dense label="Bin name" placeholder="New bin" />
              </v-list-item-content>
              <v-list-item-action>
                <v-btn icon><v-icon>mdi-check</v-icon></v-btn>
              </v-list-item-action>
            </v-list-item>
            -->

            <v-list-item 
              to="/bins/test"
              exact
              disabled
            >
              <v-list-item-action>
                <v-icon>mdi-folder</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Example jobs</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <v-divider class="mb-1" />

            <v-list-item 
              to="/bins/trash"
              exact
              disabled
            >
              <v-list-item-action>
                <v-icon>mdi-delete</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Trash</v-list-item-title>
              </v-list-item-content>
            </v-list-item>
          </v-list>
        </v-tab-item>
        <v-tab-item>
          <v-list
            expand
            nav
            dense
          >
            <v-list-item :to="{ name: 'hashes'}">
              <v-list-item-action>
                <v-icon>{{ routeIcon('hashes') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Hashes</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <v-list-item :to="{ name: 'dictionaries'}">
              <v-list-item-action>
                <v-icon>{{ routeIcon('dictionaries') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Dictionaries</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <v-list-item :to="{ name: 'pcfg'}">
              <v-list-item-action>
                <v-icon>{{ routeIcon('pcfg') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>PCFG</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <v-list-item :to="{ name: 'rules'}">
              <v-list-item-action>
                <v-icon>{{ routeIcon('rules') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Rules</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <v-list-item :to="{ name: 'charsets'}">
              <v-list-item-action>
                <v-icon>{{ routeIcon('charsets') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Charsets</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <v-list-item :to="{ name: 'masks'}">
              <v-list-item-action>
                <v-icon>{{ routeIcon('masks') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Masks</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <!--<v-list-item :to="{ name: 'files'}">-->
            <!--<v-list-item-action>-->
            <!--<v-icon>{{ routeIcon('') }}</v-icon>-->
            <!--</v-list-item-action>-->
            <!--<v-list-item-content>-->
            <!--<v-list-item-title>Encrypted files</v-list-item-title>-->
            <!--</v-list-item-content>-->
            <!--</v-list-item>-->

            <v-list-item :to="{ name: 'markovChains'}">
              <v-list-item-action>
                <v-icon>{{ routeIcon('markovChains') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Markov chains</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <!--<v-list-item :to="{ name: 'server'}">-->
            <!--<v-list-item-action>-->
            <!--<v-icon>{{ routeIcon('') }}</v-icon>-->
            <!--</v-list-item-action>-->
            <!--<v-list-item-content>-->
            <!--<v-list-item-title>Control</v-list-item-title>-->
            <!--</v-list-item-content>-->
            <!--</v-list-item>-->
          </v-list>
        </v-tab-item>
        <v-tab-item>
          <v-list
            expand
            nav
            dense
          >
            <v-list-item :to="{ name: 'hosts'}">
              <v-list-item-action>
                <v-icon>{{ routeIcon('hosts') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Hosts</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <v-list-item :to="{ name: 'templates'}">
              <v-list-item-action>
                <v-icon>{{ routeIcon('templates') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Job templates</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <v-list-item
              v-if="$userCanManageUsers()"
              :to="{ name: 'manageUsers'}"
            >
              <v-list-item-action>
                <v-icon>{{ routeIcon('manageUsers') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Manage users</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <v-list-item :to="{ name: 'myAccount'}">
              <v-list-item-action>
                <v-icon>{{ routeIcon('myAccount') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>My account</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <v-list-item
              v-if="$userCanManageUsers()"
              :to="{ name: 'server'}"
            >
              <v-list-item-action>
                <v-icon>{{ routeIcon('server') }}</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Server status</v-list-item-title>
              </v-list-item-content>
            </v-list-item>

            <v-list-item @click="logout">
              <v-list-item-action>
                <v-icon>mdi-logout-variant</v-icon>
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title>Log out</v-list-item-title>
              </v-list-item-content>
            </v-list-item>
          </v-list>
        </v-tab-item>
      </v-tabs-items>
    </v-navigation-drawer>

    <!-- TOP BAR -->

    <v-app-bar
      app
      height="64px"
      class="mainToolbar"
    >
      <v-app-bar-nav-icon @click.stop="drawer = !drawer" />
      <v-toolbar-title>{{ $store.state.project }}</v-toolbar-title>
      <!--
      <router-link :to="{ name: 'home'}" class="textLogo">
        <span class="logoSmallText" v-text="$vuetify.theme.project"></span>
      </router-link>
      -->
      <v-spacer />
      <v-btn
        icon
        @click.stop="$vuetify.theme.dark = !$vuetify.theme.dark"
      >
        <v-icon>mdi-theme-light-dark</v-icon>
      </v-btn>
      <v-badge
        color="red"
        overlap
      >
        <span
          v-if="notificationsCount > 0"
          slot="badge"
        >{{ notificationsCount }}</span>
        <v-btn
          icon
          class="ma-0"
          @click.stop="toggleNotifications"
        >
          <v-icon>{{ notificationsCount > 0 ? 'mdi-bell-ring' : 'mdi-bell' }}</v-icon>
        </v-btn>
      </v-badge>
    </v-app-bar>
    <v-content class="height100 main">
      <router-view />
    </v-content>
    <v-navigation-drawer
      v-model="rightDrawer"
      class="pa-0"
      temporary
      right
      fixed
      app
      width="400"
    >
      <v-toolbar
        flat 
        class="notifHeader"
      >
        <v-toolbar-title>Notifications</v-toolbar-title>
      </v-toolbar>
      <notifications-wrapper ref="notifWrapper" />
    </v-navigation-drawer>
  </div>
</template>

<script>
  import notifications from '@/components/notification/fc_notifications_wrapper'
  import { routeIcon } from '@/router'
  import axios from 'axios'

  export default {
    components: {
      'notifications-wrapper': notifications
    },
    data() {
      return {
        drawer: true,
        rightDrawer: false,
        notificationsCount: 0,
        navtab: parseInt(localStorage.getItem('navtab')) || 0
      }
    },
    watch: {
      navtab (val) {
        localStorage.setItem('navtab', val)
      }
    },
    beforeRouteEnter (to, from, next) {
      axios.get(window.serverAddress + '/user/isLoggedIn')
        .then(response => response.data)
        .then(userInfo => {
          if (!userInfo.loggedIn) {
            if (from.path === '/login') {
              next()
            }
            sessionStorage.setItem('loginRedirect', to.path)
            next('/login')
          } else {
            next(vm => {
              vm.$store.state.user.userData = userInfo.user
              vm.$store.state.user.loggedIn = true
            })
          }
        })
    },
    mounted: function () {
      /*
      if (!this.$store.state.user.loggedIn) {
        this.$router.push({
          name: 'login'
        })
      }
      */
      this.getNotificationsCount();
      this.interval = setInterval(function () {
        this.getNotificationsCount()
      }.bind(this), 10000)
    },
    beforeDestroy: function () {
      clearInterval(this.interval)
    },
    methods: {
      routeIcon,
      logout: function () {
        this.axios.get(this.$serverAddr + '/user/logout').then((response) => {
          this.$logoutUser()
        })
      },
      toggleNotifications: function () {
        this.rightDrawer = !this.rightDrawer;
        if (this.rightDrawer) {
          this.$refs.notifWrapper.loadNotifications()
        }
      },
      getNotificationsCount: function () {
        this.axios.get(this.$serverAddr + '/notifications/count', {
          withCredentials: true
        }).then((response) => {
          this.notificationsCount = response.data.count
        })
      }
    }
  }
</script>

<style scoped>
  .dash-link {
    display: flex;
    justify-content: center;
    padding: 1em;
  }

  .notifHeader {
    z-index: 2;
  }

  .logo {
    width: 100%;
    max-width: 150px;
    margin-bottom: -1em;
  }

  .logoText {
    color: #4c4c4c;
    text-align: center;
    padding-bottom: 15px;
    font-size: 26px;
    font-weight: 300;
    text-transform: uppercase;
  }

  .textLogoText {
    font-size: 18px;
    font-weight: 300;
  }

  .height100 {
    height: 100%;
  }

  .mainToolbar {
    z-index: 3;
  }

  .navigationDrawer {
    z-index: 6;
  }

  .main {
    padding-top: 64px !important;
  }

  .logoSmall {
    width: 100%;
    max-width: 40px;
    vertical-align: middle;
  }

  .logoSmallText {
    font-size: 24px;
    text-transform: uppercase;
    font-weight: 300;
    vertical-align: middle;
  }
</style>


<style>
  .v-list__group__items .v-list__tile {
    padding-left: 35px;
  }

</style>
