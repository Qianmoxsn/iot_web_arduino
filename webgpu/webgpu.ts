import {
  Engine3D,
  Scene3D,
  Object3D,
  Camera3D,
  ForwardRenderJob,
  LitMaterial,
  BoxGeometry,
  MeshRenderer,
  DirectLight,
  HoverCameraController,
  Color,
  FlyCameraController,
  Vector3,
  PlaneGeometry,
  BoxColliderShape,
  Collider,
  PointerEvent3D,
} from '@orillusion/core';

async function demo(){
  console.log("start demo");
  //引擎启动前需要配置开启拾取和拾取类型
  Engine3D.setting.pick.enable = true;
  // Bound: 包围盒拾取, pixel: 帧缓冲区拾取
  Engine3D.setting.pick.mode = `bound`; // or 'pixel'
  await Engine3D.init();
  let scene3D = new Scene3D();
  let cameraObj:Object3D = new Object3D();
  // 加载一个相机组件
  let camera = cameraObj.addComponent(Camera3D);
  // 根据窗口大小设置摄像机视角
  camera.perspective(60, window.innerWidth / window.innerHeight, 1, 5000.0);
  // 加载控制器组件
  let flyCameraController = cameraObj.addComponent(FlyCameraController);
  // 通过组件 setCamera 设置相机位置
  flyCameraController.setCamera(new Vector3(-90, 90, 180), new Vector3(60, 0, -10));
  // 设置鼠标移动速度
  flyCameraController.moveSpeed = 300;
  // 添加相机节点
  scene3D.addChild(cameraObj);

  let lightObj = new Object3D();
  scene3D.addChild(lightObj);
  //添加平行光
  let dl = lightObj.addComponent(DirectLight);
  //设置颜色
  dl.lightColor = new Color(1.0, 0.95, 0.84, 1.0);
  //设置强度
  dl.intensity = 20;
  // 通过 Object3D 设置光源的方向
  lightObj.rotationX = 45;
  lightObj.rotationY = -140;
  lightObj.rotationZ = 90;

  // 新建对象1
  const obj1= new Object3D();
  obj1.transform.localPosition = new Vector3(100, 50, 0);
  // 为对象添 MeshRenderer
  let mr1 = obj1.addComponent(MeshRenderer);
  // 设置几何体
  mr1.geometry = new PlaneGeometry(100, 200);
  // 设置材质
  mr1.material = new LitMaterial();
  mr1.transform.localRotation = new Vector3(0, 0, 90);
  scene3D.addChild(obj1);
  // 新建对象2
  const obj2= new Object3D();
  obj2.transform.localPosition = new Vector3(0, 50, -100);
  // 为对象添 MeshRenderer
  let mr2 = obj2.addComponent(MeshRenderer);
  // 设置几何体
  mr2.geometry = new PlaneGeometry(100, 200);
  // 设置材质
  mr2.material = new LitMaterial();
  mr2.transform.localRotation = new Vector3(90, 0, 90);
  scene3D.addChild(obj2);
  // 新建对象3
  const obj3= new Object3D();
  //obj3.transform.localPosition = new Vector3(0, 0, 0);
  // 为对象添 MeshRenderer
  let mr3 = obj3.addComponent(MeshRenderer);
  // 设置几何体
  mr3.geometry = new PlaneGeometry(200, 200);
  // 设置材质
  mr3.material = new LitMaterial();
  //mr3.transform.localRotation = new Vector3(0, 0, 0);
  scene3D.addChild(obj3);

  let scene = new Scene3D();
  // 加载 gltf 文件
  let data = await Engine3D.res.loadGltf("mesh/壁挂式空调.gltf");
  data.scaleX = data.scaleY = data.scaleZ = 40;
  data.transform.localPosition = new Vector3(0, 50, -90);
  let mr4 = scene.addComponent(MeshRenderer);
  // 设置 box geometry
  mr4.geometry = new BoxGeometry(1,1,1);
  // 添加碰撞盒检测
  let collider = scene.addComponent(Collider);
  // bound 模式需要手动设置碰撞盒样式和大小
  // 拾取精度取决于 box.geometry 和 collider.shape 的匹配程度
  collider.shape = new BoxColliderShape().setFromCenterAndSize(new Vector3(0, 0, 0), new Vector3(2, 1, 1));
  // 添加至场景
  scene3D.addChild(data);


  let scene2 = new Scene3D();
  // 加载 gltf 文件
  let data2 = await Engine3D.res.loadGltf("https://cdn.orillusion.com/PBR/DragonAttenuation/DragonAttenuation.gltf");
  data2.scaleX = data2.scaleY = data2.scaleZ = 20;
  data2.transform.localPosition = new Vector3(60, 20, 0);
  data2.transform.localRotation = new Vector3(0, -90, 0);
  // 添加至场景
  scene3D.addChild(data2);
  let scene3 = new Scene3D();
  // 加载 gltf 文件
  let data3 = await Engine3D.res.loadGltf("https://cdn.orillusion.com/PBR/SheenChair/SheenChair.gltf");
  data3.scaleX = data3.scaleY = data3.scaleZ = 40;
  data3.transform.localPosition = new Vector3(0, 0, -30);
  //data3.transform.localRotation = new Vector3(0, -90, 0);
  // 添加至场景
  scene3D.addChild(data3);
  let scene4 = new Scene3D();
  // 加载 gltf 文件
  let data4 = await Engine3D.res.loadGltf("https://cdn.orillusion.com/PBR/Duck/Duck.gltf");
  data4.scaleX = data4.scaleY = data4.scaleZ = 0.1;
  data4.transform.localPosition = new Vector3(0, 12, -30);
  data4.transform.localRotation = new Vector3(0, -90, 0);
  // 添加至场景
  scene3D.addChild(data4);

  

  // 新建前向渲染业务
  let renderJob = new ForwardRenderJob(scene3D);
  // 开始渲染
  Engine3D.startRender(renderJob);

}



demo()