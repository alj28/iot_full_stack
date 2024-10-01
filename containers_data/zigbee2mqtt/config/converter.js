const {temperature, onOff} = require('zigbee-herdsman-converters/lib/modernExtend');

const definition = {
    zigbeeModel: ['DemoTempSensor'],
    model: 'DemoTempSensor',
    vendor: 'Demo',
    description: 'Automatically generated definition',
    extend: [temperature(), onOff({"powerOnBehavior":false})],
    meta: {},
};

module.exports = definition;