const MPU6050_CODE_BEGIN = 'DEV_I2C1.MPU6050(0, 0x68)';

Blockly.JavaScript['mpu6050_acc'] = function(block) {
  var dropdown_axis = block.getFieldValue('axis');
  var code = `${MPU6050_CODE_BEGIN}.get_acceleration(${dropdown_axis})`;
  return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.JavaScript['mpu6050_gyro'] = function(block) {
  var dropdown_axis = block.getFieldValue('axis');
  var code = `${MPU6050_CODE_BEGIN}.get_gyroscope(${dropdown_axis})`;
  return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.JavaScript['mpu6050_temp'] = function(block) {
  var code = `${MPU6050_CODE_BEGIN}.get_temperature()`;
  return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.JavaScript['mpu6050_acc_set_range'] = function(block) {
  var dropdown_range = block.getFieldValue('range');
  var code = `${MPU6050_CODE_BEGIN}.set_acc_range(${dropdown_range});\n`;
  return code;
};

Blockly.JavaScript['mpu6050_gyro_set_range'] = function(block) {
  var dropdown_range = block.getFieldValue('range');
  var code = `${MPU6050_CODE_BEGIN}.set_gyro_range(${dropdown_range});\n`;
  return code;
};

