import{f as c}from"./numberFormat.773889d3.js";import"./index.cc042b8d.js";import{t as m}from"./fc_tile.7736af23.js";import{R as u}from"./resourceMonitor.3e0b4a8d.js";import{n as d}from"./index.addf402d.js";import"./index.bbc3bfb2.js";import"./timeseries.9158092b.js";import"./vendor.6aa46313.js";var v=function(){var t=this,r=t.$createElement,e=t._self._c||r;return e("div",[t.data!=null?e("v-breadcrumbs",{attrs:{items:[{text:"Hosts",to:{name:"hosts"},exact:!0},{text:t.data.domain_name}],divider:"/"}}):t._e(),e("v-container",[e("fc-tile",{staticClass:"mb-5",attrs:{title:"Host info",loading:t.data==null}},[t.data!=null?e("v-list",[e("v-list-item",[e("v-list-item-icon",[e("v-icon",[t._v("mdi-desktop-classic")])],1),e("v-list-item-content",[e("v-list-item-title",[t._v(" "+t._s(t.data.domain_name)+" ")]),e("v-list-item-subtitle",[t._v(" User: "+t._s(t.data.user.name)+" ")])],1)],1),e("v-list-item",[e("v-list-item-icon",[e("v-icon",[t._v(t._s(t.getOsIcon(t.data.os_name)))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v(" "+t._s(t.data.os_name)+" ")])],1)],1),e("v-list-item",[e("v-list-item-icon",[e("v-icon",[t._v("mdi-memory")])],1),e("v-list-item-content",[e("v-list-item-title",[t._v(" "+t._s(t.data.p_model)+" ")])],1)],1)],1):t._e()],1),e("fc-tile",{staticClass:"mb-5",attrs:{title:"Devices",loading:t.data===null}},[t.data!=null?e("v-list",t._l(t.data.devices,function(i){return e("v-list-item",{key:i.id},[e("v-list-item-icon",[i.device_info[0].temperature>100?e("v-icon",[t._v("mdi-fire")]):e("v-icon",[t._v(t._s(i.type==="GPU"?"mdi-expansion-card":"mdi-expansion-card-variant"))])],1),e("v-list-item-content",[e("v-list-item-title",[t._v(" "+t._s(i.name)+" ")]),t.$moment.utc(i.device_info[0].time).isBefore(t.$moment.utc().subtract(10,"minutes"))?e("v-list-item-subtitle",[t._v(" Inactive \u2013 no current data. ")]):e("v-list-item-subtitle",[t._v(" Speed: "+t._s(i.device_info[0].speed)+" H/s"),e("span",{directives:[{name:"show",rawName:"v-show",value:i.device_info[0].temperature>=0,expression:"device.device_info[0].temperature >= 0"}]},[t._v(", Temp: "+t._s(i.device_info[0].temperature)+" \xB0C, Util: "+t._s(i.device_info[0].utilization)+" %")])]),e("div",{staticClass:"device-charts"},[e("res-mon",{staticClass:"chart",attrs:{"usage-data":i.device_info.slice(0,30),metrics:["speed"],"unit-callback":function(a){return a+" H/s"}}}),e("res-mon",{directives:[{name:"show",rawName:"v-show",value:i.device_info[0].temperature>=0,expression:"device.device_info[0].temperature >= 0"}],staticClass:"chart",attrs:{"usage-data":i.device_info.slice(0,30),metrics:["utilization","temperature"],max:100,"right-scale":!0,"max-right":100}})],1)],1)],1)}),1):t._e()],1),e("fc-tile",{staticClass:"mb-5",attrs:{title:"Jobs",loading:t.data===null}},[t.data!==null?e("v-data-table",{attrs:{headers:t.jobHeaders,items:t.data.jobs,"footer-props":{itemsPerPageOptions:[10,25,50,100],itemsPerPageText:"Jobs per page"}},scopedSlots:t._u([{key:"item.name",fn:function(i){var a=i.item;return[e("router-link",{staticClass:"middle",attrs:{to:{name:"jobDetail",params:{id:a.id}}}},[t._v(" "+t._s(a.name)+" ")])]}},{key:"item.status",fn:function(i){var a=i.item;return[e("v-tooltip",{attrs:{top:""},scopedSlots:t._u([{key:"activator",fn:function(s){var n=s.on;return[e("span",t._g({class:a.status_type+"--text"},n),[t._v(" "+t._s(a.status_text)+" ")])]}}],null,!0)},[e("span",[t._v(t._s(a.status_tooltip))])])]}},{key:"item.progress",fn:function(i){var a=i.item;return[e("div",{staticClass:"d-flex align-center justify-end"},[e("span",{staticClass:"mr-2"},[t._v(t._s(t.progressToPercentage(a.progress)))]),e("v-progress-circular",{staticClass:"jobProgress",attrs:{size:"18",width:3,rotate:270,color:"primary",value:a.progress}})],1)]}},{key:"item.time",fn:function(i){var a=i.item;return[t._v(" "+t._s(t.$moment.utc(a.time).local().format("D.M.YYYY H:mm:ss"))+" ")]}},{key:"item.actions",fn:function(i){var a=i.item;return[e("div",{staticClass:"d-flex justify-end actionsBtns"},[e("v-tooltip",{attrs:{top:""},scopedSlots:t._u([{key:"activator",fn:function(s){var n=s.on;return[e("v-btn",t._g({staticClass:"mx-0",attrs:{icon:"",disabled:a.status!=="0"},on:{click:function(o){return t.operateJob(a.id,"start")}}},n),[e("v-icon",{attrs:{color:"success"}},[t._v(" mdi-play-circle ")])],1)]}}],null,!0)},[e("span",[t._v("Start job")])]),e("v-tooltip",{attrs:{top:""},scopedSlots:t._u([{key:"activator",fn:function(s){var n=s.on;return[e("v-btn",t._g({staticClass:"mx-0",attrs:{icon:"",disabled:a.status!=="10"},on:{click:function(o){return t.operateJob(a.id,"stop")}}},n),[e("v-icon",{attrs:{color:"error"}},[t._v(" mdi-pause-circle ")])],1)]}}],null,!0)},[e("span",[t._v("Stop job")])]),e("v-tooltip",{attrs:{top:""},scopedSlots:t._u([{key:"activator",fn:function(s){var n=s.on;return[e("v-btn",t._g({staticClass:"mx-0",attrs:{icon:"",disabled:a.status>=10},on:{click:function(o){return t.operateJob(a.id,"restart")}}},n),[e("v-icon",{attrs:{color:"info"}},[t._v(" mdi-restart ")])],1)]}}],null,!0)},[e("span",[t._v("Restart job")])])],1)]}}],null,!1,4220099537)}):t._e()],1),e("fc-tile",{staticClass:"mb-5",attrs:{title:"Workunits",loading:t.data===null}},[t.data!==null?e("v-data-table",{staticClass:"width100",attrs:{"footer-props":{itemsPerPageOptions:[10,30,60,{text:"All",value:-1}],itemsPerPageText:"Workunits per page"},headers:t.workunitsHeader,items:t.data.workunits},scopedSlots:t._u([{key:"item.job_id",fn:function(i){var a=i.item;return[e("router-link",{staticClass:"middle",attrs:{to:{name:"jobDetail",params:{id:a.job_id}}}},[t._v(" "+t._s(a.job.name||"Detail")+" ")])]}},{key:"item.start_index_real",fn:function(i){var a=i.item;return[t._v(" "+t._s(t.fmt(a.start_index_real))+" ")]}},{key:"item.hc_keyspace",fn:function(i){var a=i.item;return[t._v(" "+t._s(t.fmt(a.hc_keyspace))+" ")]}},{key:"item.time",fn:function(i){var a=i.item;return[t._v(" "+t._s(t.$moment.utc(a.time).local().format("D.M.YYYY H:mm:ss"))+" ")]}},{key:"item.retry",fn:function(i){var a=i.item;return[t._v(" "+t._s(t.yesNo(a.retry))+" ")]}},{key:"item.finished",fn:function(i){var a=i.item;return[t._v(" "+t._s(t.yesNo(a.finished))+" ")]}}],null,!1,2350630982)}):t._e()],1)],1)],1)},_=[];const f={name:"HostDetailView",components:{"fc-tile":m,"res-mon":u},data:function(){return{data:null,workunitsHeader:[{text:"Job",align:"start",value:"job_id"},{text:"Cracking time",align:"start",value:"cracking_time_str"},{text:"Generated",align:"end",value:"time"},{text:"Start index",align:"end",value:"start_index_real"},{text:"Keyspace",align:"end",value:"hc_keyspace"},{text:"Retry",align:"end",value:"retry"},{text:"Finish",align:"end",value:"finished"}],jobHeaders:[{text:"Name",align:"start",value:"name"},{text:"Attack type",value:"attack",align:"end"},{text:"Status",value:"status",align:"end"},{text:"Progress",value:"progress",align:"end"},{text:"Added",value:"time",align:"end"},{text:"Actions",value:"actions",sortable:!1,align:"end"}],jobs_statuses:[{text:"ready",code:0},{text:"finished",code:1},{text:"exhausted",code:2},{text:"malformed",code:3},{text:"timeout",code:4},{text:"running",code:10},{text:"finishing",code:12}]}},mounted:function(){this.loadData()},methods:{fmt:c,loadData:function(){this.axios.get(this.$serverAddr+"/hosts/"+this.$route.params.id).then(t=>{this.data=t.data})},yesNo:function(t){return t?"Yes":"No"},getOsIcon(t){return/(windows|microsoft)/ig.test(t)?"mdi-windows":/(linux)/ig.test(t)?"mdi-linux":/(apple|mac)/ig.test(t)?"mdi-apple":"mdi-application"},progressToPercentage:function(t){return t.toFixed()+"%"},operateJob:function(t,r){this.axios.get(this.$serverAddr+"/job/"+t+"/action",{params:{operation:r}}).then(e=>{console.log(e.data),this.loadJobs()})}}},l={};var p=d(f,v,_,!1,g,"41d233a3",null,null);function g(t){for(let r in l)this[r]=l[r]}var P=function(){return p.exports}();export{P as default};
