<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container fluid>
    <div class="users mx-auto mb-5">
      <h2>Users</h2>
      <v-card>
        <v-list two-line>
          <template v-for="(item, index) in users">
            <v-list-tile @click="">
              <v-list-tile-content>
                <v-list-tile-title>{{item.username}}
                </v-list-tile-title>
                <v-list-tile-sub-title>{{item.mail}}</v-list-tile-sub-title>
              </v-list-tile-content>
              <v-list-tile-action class="width150">
                <v-select
                  flat
                  solo
                  item-text="name"
                  item-value="id"
                  :items="userRoles"
                  label="Role"
                  :value="item.role.id"
                  @change="updateUserRole($event, item.id)"
                ></v-select>
              </v-list-tile-action>
              <v-list-tile-action>
                <v-tooltip top>
                  <v-btn icon class="mx-0" @click="deleteUser(item.id)" slot="activator">
                    <v-icon color="error">close</v-icon>
                  </v-btn>
                  <span>Delete user</span>
                </v-tooltip>
              </v-list-tile-action>
            </v-list-tile>
            <v-divider v-if="index + 1 < users.length" :key="index"></v-divider>
          </template>
        </v-list>
      </v-card>
      <div class="addBtnCont">
        <v-btn color="primary" class="addBtn" flat outline @click="userDialog=true">Add user</v-btn>
      </div>
    </div>
    <h2>User roles</h2>
    <div class="roleTable">
      <v-data-table
        class="elevation-1 roleTable"
        :headers="headers"
        :items="userRoles"
        :loading="loading"
        disable-initial-sort
        hide-actions
      >
        <template slot="items" slot-scope="props">
          <td>{{ props.item.name }}</td>
          <td class="text-xs-right">
            <v-checkbox @change="roleChange($event, props.item.id, 'ADD_NEW_JOB')"
                        v-model="props.item.ADD_NEW_JOB"></v-checkbox>
          </td>
          <td class="text-xs-right">
            <v-tooltip top>
              <v-btn icon class="mx-0" @click="deleteRole(props.item.id)" slot="activator">
                <v-icon color="error">close</v-icon>
              </v-btn>
              <span>Delete role</span>
            </v-tooltip>
          </td>
        </template>
      </v-data-table>
      <div class="addBtnCont">
        <v-btn color="primary" class="addBtn" @click="roleDialog=true" flat outline>Add role</v-btn>
      </div>
    </div>

    <v-dialog v-model="roleDialog" max-width="400px">
      <v-card>
        <v-card-title>
          <h2>Add new user role</h2>
        </v-card-title>
        <v-card-text>
          <v-text-field
            label="Role name"
            v-model="newRoleName"
          ></v-text-field>
        </v-card-text>
        <v-card-actions>
          <v-spacer></v-spacer>
          <v-btn color="primary" flat @click.stop="roleDialog=false">Cancel</v-btn>
          <v-btn color="primary" flat @click.stop="addRole">Add</v-btn>
        </v-card-actions>
        <div class="loadingOver" v-if="addingRole">
          <v-progress-circular
            size="50"
            :width="3"
            class="progress"
            indeterminate
            color="primary">
          </v-progress-circular>
        </div>
      </v-card>
    </v-dialog>


    <v-dialog v-model="userDialog" max-width="500px">
      <v-card>
        <v-card-title>
          <h2>Add new user</h2>
        </v-card-title>
        <v-card-text>
          <v-form v-model="validNewUserForm" ref="form" lazy-validation>
            <v-text-field
              label="Username"
              v-model="newUsername"
              :counter="10"
              required
            ></v-text-field>
            <v-text-field
              label="E-mail"
              v-model="newEmail"
              :rules="emailRules"
              required
            ></v-text-field>
            <v-text-field
              type="password"
              label="Password"
              v-model="newPassword"
              required
            ></v-text-field>
            <v-select
              label="User role"
              v-model="newUserRoleID"
              item-text="name"
              item-value="id"
              :items="userRoles"
              :rules="[v => !!v || 'Item is required']"
              required
            ></v-select>
          </v-form>
        </v-card-text>
        <v-card-actions>
          <v-spacer></v-spacer>
          <v-btn color="primary" flat @click.stop="userDialog=false">Cancel</v-btn>
          <v-btn color="primary" flat @click.stop="addUser" :disabled="!validNewUserForm">Add user</v-btn>
        </v-card-actions>
        <div class="loadingOver" v-if="addingUser">
          <v-progress-circular
            size="50"
            :width="3"
            class="progress"
            indeterminate
            color="primary">
          </v-progress-circular>
        </div>
      </v-card>
    </v-dialog>

  </v-container>
</template>

<script>
  export default {
    name: 'usersView',
    created: function () {
      this.loadUsers();
      this.loadRoles()
    },
    methods: {
      loadUsers() {
        this.axios.get(this.$serverAddr + '/user/').then((response) => {
          this.users = response.data.items
        })
      },
      loadRoles() {
        this.loading = true;
        this.axios.get(this.$serverAddr + '/user/role').then((response) => {
          this.userRoles = response.data.items;
          this.loading = false
        })
      },
      updateUserRole(roleId, userId) {
        this.axios.post(this.$serverAddr + '/user/role', {
          user_id: userId,
          role_id: roleId
        }).then((response) => {
          console.log(response.data);
          console.log('user role changed')
        })
      },
      roleChange(value, roleId, key) {
        this.axios.post(this.$serverAddr + '/user/role/' + roleId, {
          key: key,
          value: value
        }).then((response) => {
          console.log(response.data);
          console.log('user role changed')
        })
      },
      addRole() {
        this.addingRole = true;
        this.axios.post(this.$serverAddr + '/user/role/new', {
          name: this.newRoleName
        }).then((response) => {
          console.log(response.data);
          this.loadRoles();
          this.roleDialog = false;
          this.newRoleName = '';
          this.addingRole = false
        })
      },
      deleteRole(id) {
        this.axios.delete(this.$serverAddr + '/user/role/' + id).then((response) => {
          console.log(response.data);
          this.loadRoles()
        })
      },
      addUser() {
        this.addingUser = true;
        this.axios.post(this.$serverAddr + '/user/', {
          username: this.newUsername,
          mail: this.newEmail,
          password: this.newPassword,
          role_id: this.newUserRoleID
        }).then((response) => {
          console.log(response.data);
          this.loadUsers();
          this.userDialog = false;
          this.addingUser = false;
          this.newUsername = '';
          this.newEmail = '';
          this.newPassword = '';
          this.newUserRoleID = ''
        })
      },
      deleteUser(id) {
        this.axios.delete(this.$serverAddr + '/user/' + id).then((response) => {
          console.log(response.data);
          this.loadUsers()
        })
      },
    },
    data: function () {
      return {
        newRoleName: null,
        addingRole: false,
        roleDialog: false,
        newPassword: null,
        newUsername: null,
        newUserRoleID: null,
        newEmail: null,
        addingUser: false,
        userDialog: false,
        users: [],
        totalItems: 0,
        pagination: {},
        loading: true,
        headers: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Add new job', value: 'ADD_NEW_JOB', align: 'left'},
          {text: '', value: 'id', align: 'right'}
        ],
        userRoles: [],
        validNewUserForm: true,
        emailRules: [
          v => !!v || 'E-mail is required',
          v => /^\w+([.-]?\w+)*@\w+([.-]?\w+)*(\.\w{2,3})+$/.test(v) || 'E-mail must be valid'
        ]
      }
    }
  }
</script>

<style scoped>
  .users {
    max-width: 550px;
  }

  h2 {
    padding-bottom: 5px;
    text-align: center;
  }

  .roleTable {
    max-width: 500px;
    margin: auto;
  }

  .width150 {
    min-width: 100px;
  }

  .addBtnCont {
    text-align: right;
  }

  .addBtn {
    display: inline-block;
    margin-right: 0;
  }

  .loadingOver {
    position: absolute;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    background: white;
  }

  .loadingOver .progress {
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    margin: auto;
    position: absolute;
  }
</style>
