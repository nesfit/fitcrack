var x=Object.defineProperty,C=Object.defineProperties;var w=Object.getOwnPropertyDescriptors;var _=Object.getOwnPropertySymbols;var $=Object.prototype.hasOwnProperty,k=Object.prototype.propertyIsEnumerable;var u=(t,i,e)=>i in t?x(t,i,{enumerable:!0,configurable:!0,writable:!0,value:e}):t[i]=e,r=(t,i)=>{for(var e in i||(i={}))$.call(i,e)&&u(t,e,i[e]);if(_)for(var e of _(i))k.call(i,e)&&u(t,e,i[e]);return t},d=(t,i)=>C(t,w(i));import{t as I}from"./iconMaps.0c6dd037.js";import{n as l,t as D,s as f,r as B}from"./index.0b60da2f.js";import{m as N}from"./spyfu-vuex-helpers.esm.87da45d6.js";import{i as S,k as A}from"./vendor.6aa46313.js";import{d as j}from"./vuedraggable.umd.f0e73259.js";import{_ as J,a as L}from"./NeoLight.6d23a48a.js";var E=function(){var t=this,i=t.$createElement,e=t._self._c||i;return e("v-list-item",{class:["py-2","pr-6",{seen:t.seen}],attrs:{to:"/jobs/"+t.jobId}},[e("v-list-item-avatar",[e("v-icon",{attrs:{color:t.seen?null:t.type}},[t._v(" "+t._s(t.typeIcon(t.type))+" ")])],1),e("v-list-item-content",[e("div",{staticClass:"d-flex justify-space-between align-center mb-2"},[e("v-list-item-title",{class:["font-weight-bold",(t.seen?"":t.type)+"--text"]},[t._v(" "+t._s(t.title)+" ")]),e("v-list-item-subtitle",{staticClass:"text-end nogrow"},[t._v(" "+t._s(t.$moment(t.time).format("DD. MM. YYYY HH:mm"))+" ")])],1),e("div",{staticClass:"body-2"},[t._v(" "+t._s(t.text)+" ")])])],1)},O=[];const M={name:"Notification",props:["type","title","text","seen","time","jobId"],methods:{typeIcon:I}},p={};var W=l(M,E,O,!1,R,"451d0bba",null,null);function R(t){for(let i in p)this[i]=p[i]}var F=function(){return W.exports}(),T=function(){var t=this,i=t.$createElement,e=t._self._c||i;return e("div",[e("div",{staticClass:"notif-top d-flex align-center pa-3 pb-0"},[e("v-badge",{attrs:{value:t.count>0,content:t.count,inline:"",color:"secondary"}},[e("v-card-title",[t._v("Notifications")])],1),e("v-spacer"),e("v-btn",{attrs:{icon:""},on:{click:function(s){return t.$emit("close")}}},[e("v-icon",[t._v(" mdi-close ")])],1)],1),t.notifications===null?e("v-progress-linear",{attrs:{indeterminate:""}}):e("div",[e("v-list",t._l(t.notifications,function(s,n){return e("notification",{key:n,attrs:{type:s.type,title:s.title,text:s.text,seen:s.seen,time:s.time,"job-id":s.job_id}})}),1),e("div",{staticClass:"notif-actions d-flex pa-3"},[e("v-btn",{directives:[{name:"show",rawName:"v-show",value:t.page<t.pages,expression:"page < pages"}],attrs:{text:"",color:"primary"},on:{click:t.loadNotifications}},[e("v-icon",{attrs:{left:""}},[t._v(" mdi-timeline-text ")]),t._v(" older ")],1),e("v-spacer")],1)],1)],1)},H=[];const P={components:{notification:F},props:{count:{type:Number,default:0}},data:function(){return{notifications:[],page:1,pages:void 0}},methods:{loadNotifications(){this.axios.get(this.$serverAddr+"/notifications",{params:{page:this.page++}}).then(t=>{this.notifications=[...this.notifications,...t.data.items],this.pages=t.data.pages})}}},h={};var z=l(P,T,H,!1,U,"5afba441",null,null);function U(t){for(let i in h)this[i]=h[i]}var Y=function(){return z.exports}(),G=function(){var t=this,i=t.$createElement,e=t._self._c||i;return e("div",[e("v-subheader",[e("b",{staticClass:"mr-1"},[t._v("JOB BINS")]),e("v-progress-circular",{directives:[{name:"show",rawName:"v-show",value:t.loading,expression:"loading"}],attrs:{size:"11",width:"2",indeterminate:""}}),e("v-spacer"),t.$userCan("EDIT_ALL_JOBS")?[t.adding?e("v-btn",{key:"cancelBtn",attrs:{small:"",text:"",disabled:t.loading},on:{click:function(s){t.adding=!1}}},[t._v(" Cancel "),e("v-icon",{attrs:{right:""}},[t._v(" mdi-close ")])],1):e("v-btn",{key:"addBtn",ref:"addBtn",attrs:{small:"",text:""},on:{click:t.addBin}},[t._v(" New "),e("v-icon",{attrs:{right:""}},[t._v(" mdi-plus ")])],1)]:t._e()],2),e("v-list-item",{directives:[{name:"show",rawName:"v-show",value:t.adding,expression:"adding"}]},[e("v-list-item-action",[e("v-icon",[t._v("mdi-folder-plus")])],1),e("v-list-item-content",[e("v-text-field",{ref:"addBinInput",attrs:{"hide-details":"",dense:"",label:"Bin Name",placeholder:"New Bin"},on:{keydown:t.addBinKeyHandler},model:{value:t.newBinName,callback:function(s){t.newBinName=s},expression:"newBinName"}})],1),e("v-list-item-action",[e("v-btn",{attrs:{icon:"",disabled:t.loading},on:{click:t.createBin}},[e("v-icon",{attrs:{color:"primary"}},[t._v(" mdi-check ")])],1)],1)],1),e("draggable",{attrs:{handle:".drag-handle",animation:150,disabled:t.loading||t.canAssign||!t.$userCan("EDIT_ALL_JOBS")},on:{change:t.updateBins},model:{value:t.bins,callback:function(s){t.bins=s},expression:"bins"}},t._l(t.bins,function(s,n){var a=s.id,c=s.name,o=s.job_count;return e("v-list-item",{key:a,attrs:{to:t.canAssign?null:{name:"bins",params:{id:a}},disabled:!t.$userCan("VIEW_ALL_JOBS")&&o==0,exact:""}},[t.canAssign?e("v-list-item-action",{staticClass:"my-1 mr-7 attention",style:{animationDelay:n*250/t.bins.length+"ms"}},[parseInt(t.$route.params.id)==a?e("v-tooltip",{attrs:{right:"",color:"error"},scopedSlots:t._u([{key:"activator",fn:function(v){var m=v.on;return[e("v-btn",t._g({attrs:{icon:"",small:"",color:"error"},on:{click:function(y){return t.addOrRemoveJobs(a,!0)}}},m),[e("v-icon",[t._v(" mdi-minus ")])],1)]}}],null,!0)},[e("span",[t._v("Remove from "+t._s(c))])]):e("v-tooltip",{attrs:{right:"",color:"success"},scopedSlots:t._u([{key:"activator",fn:function(v){var m=v.on;return[e("v-btn",t._g({attrs:{icon:"",small:"",color:"success"},on:{click:function(y){return t.addOrRemoveJobs(a)}}},m),[e("v-icon",[t._v(" mdi-plus ")])],1)]}}],null,!0)},[e("span",[t._v("Add to "+t._s(c))])])],1):e("v-list-item-action",{class:[{"drag-handle":t.$userCan("EDIT_ALL_JOBS")},"my-1","reveal"]},[e("v-badge",{attrs:{content:o.toString(),value:t.$userCan("VIEW_ALL_JOBS")||o>0,overlap:"",bordered:"","offset-y":"16",color:o>0?"secondary":"error"}},[e("v-icon",[t._v(t._s(t.$userCan("EDIT_ALL_JOBS")?"mdi-drag":"mdi-folder-outline"))])],1)],1),e("v-list-item-content",[e("v-list-item-title",[t._v(t._s(c))])],1)],1)}),1)],1)},V=[];const q={components:{draggable:j},data(){return{adding:!1,newBinName:""}},computed:d(r(r({},N("binInterface",D(["bins"]))),S("binInterface",["loading","selectedJobs"])),{canAssign(){return this.selectedJobs.length>0&&this.$route.params.id!=="trash"&&this.$userCan("EDIT_ALL_JOBS")}}),mounted(){this.load()},methods:d(r({},A("binInterface",["load","create","move","assign"])),{addBin(){this.adding=!0,this.$nextTick(()=>this.$refs.addBinInput.focus())},async createBin(){this.newBinName===""||this.loading||(await this.create(this.newBinName),this.newBinName="",this.adding=!1,setTimeout(()=>this.$refs.addBtn.$el.focus(),25))},updateBins({moved:{newIndex:t,oldIndex:i,element:{id:e}}}){this.move({id:e,position:t,oldIndex:i})},addBinKeyHandler({keyCode:t}){switch(t){case 13:this.createBin();break;case 27:this.adding=!1;break}},addOrRemoveJobs(t,i=!1){this.assign({id:t,payload:{[i?"exclude":"include"]:this.selectedJobs.map(e=>e.id)},dirty:i})}})},g={};var K=l(q,G,V,!1,Q,"968cdfcc",null,null);function Q(t){for(let i in g)this[i]=g[i]}var X=function(){return K.exports}(),Z=function(){var t=this,i=t.$createElement,e=t._self._c||i;return e("div",{staticClass:"height100"},[e("v-navigation-drawer",{staticClass:"navigationDrawer",attrs:{persistent:"","enable-resize-watcher":"",fixed:"",app:"",width:"290"},model:{value:t.drawer,callback:function(s){t.drawer=s},expression:"drawer"}},[e("router-link",{attrs:{to:{name:"home"}}},[t.$vuetify.theme.dark?e("img",{staticClass:"mx-auto px-2 mt-2 d-block logo",attrs:{src:J,alt:"logo"}}):e("img",{staticClass:"mx-auto px-2 mt-2 d-block logo",attrs:{src:L,alt:"logo"}})]),e("div",{staticClass:"dash-link"},[e("v-btn",{attrs:{to:"/",text:""}},[e("v-icon",{attrs:{left:""}},[t._v(" mdi-view-dashboard ")]),t._v(" Dashboard ")],1)],1),e("v-divider"),e("v-tabs",{staticClass:"notrans",attrs:{"icons-and-text":"",grow:""},model:{value:t.navtab,callback:function(s){t.navtab=s},expression:"navtab"}},[e("v-tab",[t._v(" Jobs "),e("v-icon",[t._v("mdi-briefcase-outline")])],1),e("v-tab",[t._v(" Library "),e("v-icon",[t._v("mdi-folder-outline")])],1),e("v-tab",[t._v(" System "),e("v-icon",[t._v("mdi-settings-outline")])],1)],1),e("v-tabs-items",{staticClass:"notrans",model:{value:t.navtab,callback:function(s){t.navtab=s},expression:"navtab"}},[e("v-tab-item",{attrs:{eager:""}},[e("v-list",{attrs:{expand:"",nav:"",dense:""}},[t.$userCan("ADD_NEW_JOB")?e("v-list-item",{attrs:{to:{name:"addJob"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("addJob")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Add Job")])],1)],1):t._e(),e("v-list-item",{attrs:{to:{name:"batches"},exact:""}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("batches")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Batches")])],1)],1),e("v-list-item",{attrs:{to:{name:"jobs"},exact:""}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("jobs")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v(t._s(t.$userCan("VIEW_ALL_JOBS")?"All":"My")+" Jobs")])],1)],1),e("v-divider",{staticClass:"mb-1"}),e("bins"),e("v-divider",{staticClass:"my-1"}),e("v-list-item",{attrs:{to:"/bins/trash",exact:""}},[e("v-list-item-action",[e("v-icon",[t._v("mdi-delete")])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Trash")])],1)],1)],1)],1),e("v-tab-item",[e("v-list",{attrs:{expand:"",nav:"",dense:""}},[e("v-list-item",{attrs:{to:{name:"hashes"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("hashes")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Hashes")])],1)],1),e("v-list-item",{attrs:{to:{name:"dictionaries"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("dictionaries")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Dictionaries")])],1)],1),e("v-list-item",{attrs:{to:{name:"pcfg"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("pcfg")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("PCFG")])],1)],1),e("v-list-item",{attrs:{to:{name:"rules"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("rules")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Rules")])],1)],1),e("v-list-item",{attrs:{to:{name:"charsets"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("charsets")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Charsets")])],1)],1),e("v-list-item",{attrs:{to:{name:"masks"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("masks")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Masks")])],1)],1),e("v-list-item",{attrs:{to:{name:"markovChains"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("markovChains")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Markov Chains")])],1)],1),e("v-list-item",{attrs:{to:{name:"files"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("files")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Encrypted Files")])],1)],1)],1)],1),e("v-tab-item",[e("v-list",{attrs:{expand:"",nav:"",dense:""}},[e("v-list-item",{attrs:{to:{name:"hosts"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("hosts")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Hosts")])],1)],1),e("v-list-item",{attrs:{to:{name:"templates"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("templates")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Job Templates")])],1)],1),t.$userCan("MANAGE_USERS")?e("v-list-item",{attrs:{to:{name:"manageUsers"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("manageUsers")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Manage Users")])],1)],1):t._e(),t.$userCan("MANAGE_USERS")?e("v-list-item",{attrs:{to:{name:"server"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("server")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Server Monitor")])],1)],1):t._e(),e("v-list-item",{attrs:{to:{name:"settings"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("settings")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Settings")])],1)],1),e("v-list-item",{attrs:{to:{name:"transfer"}}},[e("v-list-item-action",[e("v-icon",[t._v(t._s(t.routeIcon("transfer")))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v("Data Transfer")])],1)],1)],1)],1)],1)],1),e("v-app-bar",{class:["mainToolbar",{"very-important-must-not-miss":t.daemonWarning}],attrs:{app:"",height:"64px",color:t.daemonWarning?"error":"",dark:t.daemonWarning}},[e("v-app-bar-nav-icon",{on:{click:function(s){s.stopPropagation(),t.drawer=!t.drawer}}}),e("v-toolbar-title",[t._v(t._s(t.$store.state.project))]),e("v-spacer"),t.daemonWarning?e("v-row",{staticClass:"align-center"},[e("v-col",{staticClass:"text-center"},[e("v-icon",{staticClass:"important-must-not-miss",attrs:{large:"",left:""}},[t._v(" mdi-alert-octagon ")]),e("span",{staticClass:"mr-4"},[t._v(" Some Daemons are not running! ")]),e("v-btn",{attrs:{light:""},on:{click:function(s){t.daemonDetails=!0}}},[t._v(" Details ")]),e("v-btn",{attrs:{icon:""},on:{click:function(s){t.daemonWarning=!1}}},[e("v-icon",[t._v(" mdi-close-circle ")])],1)],1)],1):t._e(),e("v-spacer"),e("v-badge",{attrs:{value:t.notificationsCount>0,content:t.notificationsCount,color:"red",overlap:"",left:"",top:"","offset-y":"18"}},[e("v-btn",{attrs:{large:"",rounded:"",icon:""},on:{click:function(s){return s.stopPropagation(),t.toggleNotifications.apply(null,arguments)}}},[e("v-icon",[t._v(" "+t._s(t.notificationsCount>0?"mdi-bell":"mdi-bell-outline")+" ")])],1)],1),e("v-menu",{attrs:{"close-on-content-click":!1,transition:"slide-y-transition"},scopedSlots:t._u([{key:"activator",fn:function(s){var n=s.on;return[e("v-btn",t._g({staticClass:"pr-2 pl-3",attrs:{large:"",rounded:"",text:""}},n),[e("div",{staticClass:"text-none text-end subtitle-2 mr-2"},[t._v(" "+t._s(t.user.username)+" ")]),e("v-avatar",{attrs:{color:t.daemonWarning?"secondary":"primary",size:"32"}},[e("span",{staticClass:"white--text"},[t._v(" "+t._s(t.user.username.split(" ").map(function(a){return a[0]}).join(""))+" ")])])],1)]}}]),model:{value:t.userFlyout,callback:function(s){t.userFlyout=s},expression:"userFlyout"}},[e("v-card",{attrs:{"min-width":"250"}},[e("v-card-text",{staticClass:"text--primary d-flex align-center"},[e("div",[e("div",{staticClass:"subtitle-2"},[t._v(" "+t._s(t.user.username)+" ")]),e("div",{staticClass:"caption"},[t._v(" "+t._s(t.user.mail)+" ")])]),e("v-spacer"),e("v-btn",{staticClass:"ml-4",attrs:{text:""},on:{click:function(s){t.roles=!t.roles}}},[t._v(" "+t._s(t.user.role.name)+" "),e("v-icon",{attrs:{right:""}},[t._v(" "+t._s(t.roles?"mdi-chevron-up":"mdi-chevron-down")+" ")])],1)],1),e("v-expand-transition",[e("div",{directives:[{name:"show",rawName:"v-show",value:t.roles,expression:"roles"}]},[e("v-card-text",{staticClass:"py-0"},[e("div",{staticClass:"subtitle-1"},[t._v(" Roles ")]),e("v-list",{attrs:{dense:""}},t._l(t.roleList,function(s){return e("v-list-item",{key:s},[e("v-list-item-title",{staticClass:"text-capitalize"},[t._v(" "+t._s(s)+" ")])],1)}),1)],1)],1)]),e("v-card-actions",[e("v-btn",{staticClass:"mr-2",attrs:{text:"",to:{name:"myAccount"}},on:{click:function(s){t.userFlyout=!1}}},[e("v-icon",{attrs:{left:""}},[t._v(" "+t._s(t.routeIcon("myAccount"))+" ")]),e("span",[t._v("My Account")])],1),e("v-spacer"),e("v-btn",{staticClass:"ml-2",attrs:{text:""},on:{click:function(s){return s.stopPropagation(),t.$store.dispatch("signOut")}}},[e("span",[t._v("Sign out")]),e("v-icon",{attrs:{right:""}},[t._v(" mdi-logout-variant ")])],1)],1)],1)],1)],1),e("v-main",{staticClass:"height100 main"},[e("transition",{attrs:{name:"route",mode:"out-in"}},[e("router-view")],1)],1),e("v-navigation-drawer",{staticClass:"pa-0",attrs:{temporary:"",right:"",fixed:"",app:"",width:"450"},model:{value:t.rightDrawer,callback:function(s){t.rightDrawer=s},expression:"rightDrawer"}},[e("notifications-wrapper",{ref:"notifWrapper",attrs:{count:t.notificationsCount},on:{close:function(s){t.rightDrawer=!1}}})],1),e("v-dialog",{attrs:{"overlay-color":"red darken-4","overlay-opacity":.8,"max-width":"550"},model:{value:t.daemonDetails,callback:function(s){t.daemonDetails=s},expression:"daemonDetails"}},[e("v-card",[e("v-card-title",[t._v(" Daemons not running ")]),e("v-card-text",{staticClass:"pb-0"},[t._v(" The server daemons are services that carry out regular routines and "),e("strong",[t._v("are vital")]),t._v(" for the inner workings of Fitcrack. It seems that, unfortunately, some have crashed. Here's a list of inactive daemons: ")]),e("v-card-text",{staticClass:"py-0"},[e("v-list",{attrs:{dense:""}},t._l(t.fallenDaemons,function(s){return e("v-list-item",{key:s},[e("v-list-item-title",[t._v(" "+t._s(s)+" ")])],1)}),1)],1),e("v-card-text",[e("v-sheet",{staticClass:"pa-4 mb-2",attrs:{dark:"",color:"error"}},[t._v(" Follow these steps to summon them again: "),e("ol",[e("li",[t._v("Access your server's shell (ex. via ssh)")]),e("li",[t._v(" Run "),e("code",[t._v("/home/"),e("em",[t._v("<BOINC user>")]),t._v("/projects/"+t._s(t.$store.state.project)+"/bin/start")]),t._v(" as BOINC. BOINC user is "),e("code",[t._v("boincadm")]),t._v(" by default. ")])])]),e("span",[t._v(" After starting successfully, you can dismiss the warning. ")])],1)],1)],1),e("v-fab-transition",[t.transferCount>0&&t.$route.name!=="transfer"?e("v-btn",{staticClass:"z-top",attrs:{fab:"",fixed:"",bottom:"",left:"",color:"primary",to:{name:"transfer"}}},[e("v-badge",{attrs:{content:t.transferCount,color:"secondary"}},[e("v-icon",[t._v(" mdi-dolly ")])],1)],1):t._e()],1)],1)},tt=[];const et={components:{"notifications-wrapper":Y,bins:X},data(){return{drawer:!0,rightDrawer:!1,userFlyout:!1,roles:!1,daemonWarning:!1,daemonDetails:!1,fallenDaemons:[],notificationsCount:0,navtab:parseInt(localStorage.getItem("navtab"))||0}},computed:{user(){return this.$store.state.user.userData},roleList(){return Object.entries(this.user.role).filter(t=>t[1]===!0).map(t=>t[0]).map(t=>t.split("_").join(" ").toLowerCase())},transferCount(){return this.$store.state.transfer.jobs.length}},watch:{$route(t){t.meta.navtab!==void 0&&(this.navtab=t.meta.navtab)},navtab(t){localStorage.setItem("navtab",t)}},async beforeRouteEnter(t,i,e){try{await f.dispatch("resume")?e():(i.path==="/login"&&e(),sessionStorage.setItem("loginRedirect",t.path),e("/login"))}catch{f.commit("_setUser",null),e("/login")}},mounted:function(){this.getNotificationsCount(),this.checkDaemons(),this.ninterval=setInterval(this.getNotificationsCount,1e4),this.dinterval=setInterval(this.checkDaemons,6e4)},beforeDestroy:function(){clearInterval(this.ninterval),clearInterval(this.dinterval)},methods:{routeIcon:B,toggleNotifications:function(){this.rightDrawer=!this.rightDrawer,this.rightDrawer&&this.$refs.notifWrapper.loadNotifications()},getNotificationsCount:function(){this.axios.get(this.$serverAddr+"/notifications/count",{withCredentials:!0}).then(t=>{this.notificationsCount=t.data.count})},async checkDaemons(){const t=await this.axios.get(`${this.$serverAddr}/serverInfo/info`).then(i=>i.data.subsystems);this.fallenDaemons=t.filter(i=>i.status!=="running").map(i=>i.name),this.daemonWarning=this.fallenDaemons.length>0,this.daemonWarning||(this.daemonDetails=!1)}}},b={};var st=l(et,Z,tt,!1,it,"b1a55caa",null,null);function it(t){for(let i in b)this[i]=b[i]}var mt=function(){return st.exports}();export{mt as default};
