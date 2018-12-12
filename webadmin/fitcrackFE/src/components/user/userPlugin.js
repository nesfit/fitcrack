/*
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
*/

const UserPlugin = {
  install(Vue, options) {
    Vue.prototype.$logoutUser = function () {
      this.$store.user = {
        'userData': {
          'id': null,
          'username': null,
          'mail': null,
          'role': {
            'MANAGE_USERS': false,
            'ADD_NEW_JOB': false,
            'UPLOAD_DICTIONARIES': false,
            'VIEW_ALL_PACKAGES': false,
            'EDIT_ALL_PACKAGES': false,
            'OPERATE_ALL_PACKAGES': false,
            'ADD_USER_PERMISSIONS_TO_PACKAGE': false
          }
        },
        'loggedIn': false
      };
      console.log('logged OUT!!!');
      this.$router.push({
        name: 'login'
      })
    };

    Vue.prototype.$logInUser = function (user) {
      console.log(this.$store.loggedInLink);
      this.$store.user.userData = user;
      this.$store.user.loggedIn = true;
      console.log('logged IN!!!');
      if (this.$store.loggedInLink === null || this.$store.loggedInLink.name === 'login') {
        this.$router.push({
          name: 'home'
        })
      } else {
        this.$router.push({
          path: this.$store.loggedInLink.path
        })
      }
    };


    Vue.prototype.$needLogin = true

    Vue.prototype.$serverAddr = window.serverAddress
    // Vue.prototype.$serverAddr = 'http://147.229.14.131/api';

    Vue.prototype.$user = function () {
      return this.user
    };

    Vue.prototype.$userCanManageUsers = function () {
      return this.$store.user.userData.role.MANAGE_USERS
    };

    Vue.prototype.$userCanAddJob = function () {
      return this.$store.user.userData.role.ADD_NEW_JOB
    }

    Vue.prototype.$error = function (message) {
      this.axios.interceptors.response.handlers["0"].rejected({message: message})
    }

    Vue.prototype.$success = function (message) {
      this.axios.interceptors.response.handlers["0"].fulfilled(
        {data:
            { status: true,
              message: message
            }
        })
    }
  }
};

export default UserPlugin
