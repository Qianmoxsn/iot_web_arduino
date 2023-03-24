import {
    Engine3D,
    Scene3D,
    PlaneGeometry,
    GUIHelp,
    Vector3,
    KelvinUtil,
    Object3D,
    Camera3D,
    CameraUtil,
    webGPUContext,
    DirectLight,
    OutlinePost,
    outlinePostManager,
    FlyCameraController,
    ForwardRenderJob,
    LitMaterial,
    MeshRenderer,
    BoxColliderShape,
    Collider,
    BoxGeometry,
    ComponentBase,
    Color,
    PointerEvent3D,
    SphereGeometry,
  } from "@orillusion/core";
  let n = 0;
  
  import ipconfig from "../ipconfig.json" assert { type: "json" };
  
  //服务端ip修改仅需更改此处
  var ws_server_ip = ipconfig.ws_server_ip_dev_local;
  // var ws_server_ip = ipconfig.ws_server_ip_dev;
  //var ws_server_ip = ipconfig.ws_server_ip;
  
  let ws = new WebSocket("ws://" + ws_server_ip + ":8080");
  
  
  // document.getElementById("btnon").onclick = function () {
  //   ws.send("5678");
  //   console.log(5678);
  // };
  // document.getElementById("btnoff").onclick = function () {
  //   ws.send("1234");
  // };
  
  export default class TouchDemo {
    scene: Scene3D;
    lightObj: Object3D;
    cameraObj: Object3D;
  
    camera: Camera3D;
  
    constructor() {}
  
    async run() {
      console.log("start demo");
      Engine3D.engineSetting.pickerMode.enable = true;
      Engine3D.engineSetting.pickerMode.mode = `bound`;
      await Engine3D.init();
      Engine3D.engineSetting.renderSetting.postProcessing.outline.outlinePixel = 0;
      Engine3D.engineSetting.renderSetting.postProcessing.outline.fadeOutlinePixel = 0;
  
    //var ws_server_ip = ipconfig.ws_server_ip_dev_local;
    let ws = new WebSocket("ws://" + ws_server_ip + ":8080");
    ws.onopen = function () {
      ws.send("[C]You are connected with broswer");
    };
    ws.onmessage = function (evt) {
      console.log(evt.data);
      if (evt.data.slice(1, 2) === "T") {
        let temp = evt.data.slice(3, 7);
        document.getElementById("temp").innerHTML = temp;
        let hum = evt.data.slice(8, 10);
        document.getElementById("hum").innerHTML = hum;
      } else if (evt.data.slice(1, 2) === "L" || evt.data.slice(1, 2) === "W") {
        if (evt.data.slice(3, 7) == "5678") {
          Engine3D.engineSetting.renderSetting.postProcessing.outline.outlinePixel = 2;
          Engine3D.engineSetting.renderSetting.postProcessing.outline.fadeOutlinePixel = 4;
          n=1;
        } else if (evt.data.slice(3, 7) == "1234") {
          Engine3D.engineSetting.renderSetting.postProcessing.outline.outlinePixel = 0;
          Engine3D.engineSetting.renderSetting.postProcessing.outline.fadeOutlinePixel = 0;
          n=0;
        }
      }
    };
  
  
      this.scene = new Scene3D();
      // this.cameraObj = new Object3D();
      // this.camera = this.cameraObj.addComponent(Camera3D)
      // this.scene.addChild(this.cameraObj);
      // this.camera.lookAt(new Vector3(5, 5, 25), new Vector3(-3, 0, -5));
      // this.camera.perspective(60, window.innerWidth / window.innerHeight, 1, 10000.0);
      //相机控制
      let camera = CameraUtil.createCamera3DObject(this.scene);
      camera.perspective(60, webGPUContext.aspect, 1, 5000.0);
      let ctrl = camera.object3D.addComponent(FlyCameraController);
      ctrl.setCamera(new Vector3(-90, 90, 180), new Vector3(60, 0, -10));
      ctrl.moveSpeed = 300;
      await this.initScene(this.scene);
      //let box = this.createBox(0, 50, -80);
      //let sphere = this.createSphere(2, 0, 0);
      let floor1 = this.createFloor(100, 50, 0, 0, 0, 90, 100, 200);
      let floor2 = this.createFloor(0, 50, -100, 90, 0, 90, 100, 200);
      let floor3 = this.createFloor(0, 0, 0, 0, 0, 0, 200, 200);
      let scene = new Scene3D();
      // 加载 gltf 文件
      let data = await Engine3D.res.loadGltf("mesh/壁挂式空调.gltf");
      data.scaleX = data.scaleY = data.scaleZ = 40;
      data.transform.y = 50;
      data.transform.x = 0;
      data.transform.z = -90;
      outlinePostManager.setOutlineList([[data]], [new Color(1, 0.2, 0, 1)]);
  
      let shape: BoxColliderShape = new BoxColliderShape().setFromCenterAndSize(
        new Vector3(0, 0, 0),
        new Vector3(2, 1, 1)
      );
      //加一个碰撞盒子。
      let collider = data.addComponent(Collider);
      collider.shape = shape;
      // 为对象添 MeshRenderer
      let mr: MeshRenderer = data.addComponent(MeshRenderer);
      // 更改对象颜色
      // 添加至场景
      this.scene.addChild(data);
  
      let scene2 = new Scene3D();
      // 加载 gltf 文件
      let data2 = await Engine3D.res.loadGltf(
        "https://cdn.orillusion.com/PBR/DragonAttenuation/DragonAttenuation.gltf"
      );
      data2.scaleX = data2.scaleY = data2.scaleZ = 20;
      data2.transform.y = 20;
      data2.transform.x = 60;
      data2.transform.z = 0;
      data2.rotationX = 0;
      data2.rotationY = -90;
      data2.rotationZ = 0;
      // 添加至场景
      this.scene.addChild(data2);
  
      let scene3 = new Scene3D();
      // 加载 gltf 文件
      let data3 = await Engine3D.res.loadGltf(
        "https://cdn.orillusion.com/PBR/SheenChair/SheenChair.gltf"
      );
      data3.scaleX = data3.scaleY = data3.scaleZ = 40;
      data3.transform.y = 0;
      data3.transform.x = 0;
      data3.transform.z = -30;
      data3.rotationX = 0;
      data3.rotationY = 0;
      data3.rotationZ = 0;
      // 添加至场景
      this.scene.addChild(data3);
  
      let scene4 = new Scene3D();
      // 加载 gltf 文件
      let data4 = await Engine3D.res.loadGltf(
        "https://cdn.orillusion.com/PBR/Duck/Duck.gltf"
      );
      data4.scaleX = data4.scaleY = data4.scaleZ = 0.1;
      data4.transform.y = 12;
      data4.transform.x = 0;
      data4.transform.z = -30;
      data4.rotationX = 0;
      data4.rotationY = -90;
      data4.rotationZ = 0;
      // 添加至场景
      this.scene.addChild(data4);
  
      let renderJob = new ForwardRenderJob(this.scene);
      renderJob.addPost(new OutlinePost());
      Engine3D.startRender(renderJob);
  
      // 统一监听点击事件
      Engine3D.pickFire.addEventListener(
        PointerEvent3D.PICK_CLICK,
        this.onPick,
        this
      );
    }
  
    async initScene(scene: Scene3D) {
      /******** light *******/
      {
        this.lightObj = new Object3D();
        this.lightObj.x = 70;
        this.lightObj.y = 50;
        this.lightObj.z = -40;
        this.lightObj.rotationX = 45;
        this.lightObj.rotationY = -140;
        this.lightObj.rotationZ = 90;
        let lc = this.lightObj.addComponent(DirectLight);
        lc.lightColor = KelvinUtil.color_temperature_to_rgb(5355);
        lc.castShadow = true;
        lc.intensity = 5.7;
        scene.addChild(this.lightObj);
      }
      //this.createPlane(scene);
  
      return true;
    }
  
    createBox(x: number, y: number, z: number) {
      let boxObj = new Object3D();
      boxObj.transform.localPosition = new Vector3(x, y, z);
  
      let size: number = 40;
      let shape: BoxColliderShape = new BoxColliderShape().setFromCenterAndSize(
        new Vector3(0, 0, 0),
        new Vector3(size, size, size)
      );
      //加一个碰撞盒子。
      let collider = boxObj.addComponent(Collider);
      collider.shape = shape;
      // 为对象添 MeshRenderer
      let mr: MeshRenderer = boxObj.addComponent(MeshRenderer);
      // 设置几何体
      mr.geometry = new BoxGeometry(size, size, size);
      // 设置材质
      mr.material = new LitMaterial();
      // 更改对象颜色
      mr.material.baseColor = new Color(1, 1, 1, 1);
      outlinePostManager.setOutlineList([[boxObj]], [new Color(1, 0.2, 0, 1)]);
      this.scene.addChild(boxObj);
      return boxObj;
    }
  
    createFloor(
      x: number,
      y: number,
      z: number,
      a: number,
      b: number,
      c: number,
      d: number,
      e: number
    ) {
      let boxObj = new Object3D();
      boxObj.transform.localPosition = new Vector3(x, y, z);
      // 为对象添 MeshRenderer
      let mr: MeshRenderer = boxObj.addComponent(MeshRenderer);
      // 设置几何体
      mr.geometry = new PlaneGeometry(d, e);
      // 设置材质
      mr.material = new LitMaterial();
      // 更改对象颜色为白色
      //mr.material.baseColor = new Color( 10, 10, 10, 1);
      boxObj.rotationX = a;
      boxObj.rotationY = b;
      boxObj.rotationZ = c;
      this.scene.addChild(boxObj);
      return boxObj;
    }
  
    createSphere(x: number, y: number, z: number) {
      let sphereObj = new Object3D();
      sphereObj.transform.localPosition = new Vector3(x, y, z);
  
      let size: number = 2;
      let shape: BoxColliderShape = new BoxColliderShape().setFromCenterAndSize(
        new Vector3(0, 0, 0),
        new Vector3(size, size, size)
      );
      //加一个碰撞盒子。
      let collider = sphereObj.addComponent(Collider);
      collider.shape = shape;
      // 为对象添 MeshRenderer
      let mr: MeshRenderer = sphereObj.addComponent(MeshRenderer);
      // 设置几何体
      mr.geometry = new SphereGeometry(size / 2, 20, 20);
      // 设置材质
      mr.material = new LitMaterial();
      this.scene.addChild(sphereObj);
      return sphereObj;
    }
  
    onPick(e: PointerEvent3D) {
      console.log("onClick:", e);
      //let mr: MeshRenderer = e.target.getComponent(MeshRenderer);
      //mr.material.baseColor = Color.random();
      //ws.send("5678");
      //console.log('haha');
      //let butoff = document.getElementById("btnoff");
      //butoff.onclick();
      if (n % 2 == 0) {
        ws.send("5678");
        console.log("5678");
        Engine3D.engineSetting.renderSetting.postProcessing.outline.outlinePixel = 2;
        Engine3D.engineSetting.renderSetting.postProcessing.outline.fadeOutlinePixel = 4;
        n=1;
      } else {
        ws.send("1234");
        console.log("1234");
        Engine3D.engineSetting.renderSetting.postProcessing.outline.outlinePixel = 0;
        Engine3D.engineSetting.renderSetting.postProcessing.outline.fadeOutlinePixel = 0;
        n=0;
      }
    }
  }
  new TouchDemo().run();