import { Engine3D, Scene3D, PlaneGeometry,
    GUIHelp,Vector3, Object3D, Camera3D, ForwardRenderJob, HDRLitMaterial, MeshRenderer, BoxColliderShape, Collider, BoxGeometry, ComponentBase, Color, PointerEvent3D, SphereGeometry } from "@orillusion/core";

//websocket
let ws = new WebSocket("ws://localhost:8080");
ws.onopen = function () {
  ws.send("Here is the client! You are connected!");
};
ws.onmessage = function (evt) {
    console.log(evt.data);
    if (evt.data.slice(1, 2) === "T") {
      let temp = evt.data.slice(4, 8);
      document.getElementById("temp").innerHTML = temp;
      let hum = evt.data.slice(9, 11);
      document.getElementById("hum").innerHTML = hum;
    } else if (evt.data.slice(1, 2) === "L" || evt.data.slice(1, 2) === "W") {
      if (evt.data.slice(3,7) == "5678") {
        document.getElementById("btnon").style.backgroundColor = "#a9e089";
        document.getElementById("btnoff").style.backgroundColor = "#f9f9f9";
      } else if (evt.data.slice(3,7) == "1234") {
        document.getElementById("btnoff").style.backgroundColor = "#ff9d42";
        document.getElementById("btnon").style.backgroundColor = "#f9f9f9";
      }
    }
  };
  
  document.getElementById("btnon").onclick = function () {
    ws.send("5678");
  };
  document.getElementById("btnoff").onclick = function () {
    ws.send("1234");
  };

  export default class TouchDemo {
      scene: Scene3D;
  
      cameraObj: Object3D;
  
      camera: Camera3D;
  
      constructor() { }
        
      async run() {
          console.log('start demo');
          
          Engine3D.engineSetting.pickerMode.enable = true;
          Engine3D.engineSetting.pickerMode.mode = `bound`;
          await Engine3D.init();
          this.scene = new Scene3D();
          this.cameraObj = new Object3D();
          this.camera = this.cameraObj.addComponent(Camera3D)
          this.scene.addChild(this.cameraObj);
          this.camera.lookAt(new Vector3(5, 5, 25), new Vector3(-3, 0, -5));
          this.camera.perspective(60, window.innerWidth / window.innerHeight, 1, 10000.0);
  
          let box = this.createBox(-2, 0, 0);
          //let sphere = this.createSphere(2, 0, 0);
            let floor1 = this.createFloor(0, 0, -10, 90, 0, 90,10,20);
            let floor2 = this.createFloor(-10, 0, 0, 180, 0, 90,10,20);
            let floor3 = this.createFloor(0, -5, 0, 0, 0, 0,20,20);
            let scene = new Scene3D();
            // 加载 gltf 文件
            let data = await Engine3D.res.loadGltf('壁挂式空调.gltf');
            data.scaleX = data.scaleY = data.scaleZ = 4;
            // 添加至场景
            this.scene.addChild(data);
          let renderJob = new ForwardRenderJob(this.scene);
          Engine3D.startRender(renderJob);

          

          // 统一监听点击事件
          Engine3D.pickFire.addEventListener(PointerEvent3D.PICK_CLICK, this.onPick, this);
      }
  
      createBox(x: number, y: number, z: number) {
        let boxObj = new Object3D();
          boxObj.transform.localPosition = new Vector3(x, y, z);
  
          let size: number = 2;
          let shape: BoxColliderShape = new BoxColliderShape().setFromCenterAndSize(new Vector3(0, 0, 0), new Vector3(size, size, size));
          //加一个碰撞盒子。
          let collider = boxObj.addComponent(Collider);
          collider.shape = shape;
          // 为对象添 MeshRenderer
          let mr: MeshRenderer = boxObj.addComponent(MeshRenderer);
          // 设置几何体
          mr.geometry = new BoxGeometry(size, size, size);
          // 设置材质
          mr.material = new HDRLitMaterial();
          this.scene.addChild(boxObj);
          return boxObj;
      }

      createFloor(x: number, y: number, z: number, a: number, b: number, c: number, d: number, e: number) {
        let boxObj = new Object3D();
        boxObj.transform.localPosition = new Vector3(x, y, z);
        // 为对象添 MeshRenderer
        let mr: MeshRenderer = boxObj.addComponent(MeshRenderer);
        // 设置几何体
        mr.geometry = new PlaneGeometry(d, e);
        // 设置材质
        mr.material = new HDRLitMaterial();
        boxObj.rotationX=a;
        boxObj.rotationY=b;
        boxObj.rotationZ=c;
        this.scene.addChild(boxObj);
        return boxObj;
    }
  
      createSphere(x: number, y: number, z: number){
          let sphereObj = new Object3D();
          sphereObj.transform.localPosition = new Vector3(x, y, z);
  
          let size: number = 2;
          let shape: BoxColliderShape = new BoxColliderShape().setFromCenterAndSize(new Vector3(0, 0, 0), new Vector3(size, size, size));
          //加一个碰撞盒子。
          let collider = sphereObj.addComponent(Collider);
          collider.shape = shape;
          // 为对象添 MeshRenderer
          let mr: MeshRenderer = sphereObj.addComponent(MeshRenderer);
          // 设置几何体
          mr.geometry = new SphereGeometry(size/2, 20, 20);
          // 设置材质
          mr.material = new HDRLitMaterial();
          this.scene.addChild(sphereObj);
          return sphereObj;
      }
  
      onPick(e: PointerEvent3D) {
          console.log('onClick:', e);
          let mr: MeshRenderer = e.target.getComponent(MeshRenderer);
          mr.material.baseColor = Color.random();
      }
  }
  new TouchDemo().run();
  
  
  