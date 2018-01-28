script {
  nodes = {
    {
      name = 'components/attackcomponent',
    },
    {
      name = 'number',
      loadArguments = {
        0.69999998807907,
      },
    },
    {
      name = 'number',
      loadArguments = {
        1,
      },
    },
    {
      name = 'bool',
      loadArguments = {
        true,
      },
    },
    {
      name = 'bool',
      loadArguments = {
        false,
      },
    },
    {
      name = 'script/scriptoutput',
      loadArguments = {
        {
          5,
          'Table',
        },
      },
    },
    {
      name = 'number',
      loadArguments = {
        0,
      },
    },
    {
      name = 'entity/getattacktarget',
    },
    {
      name = 'entity/lookatentity',
    },
    {
      name = 'entity/playanimation',
    },
    {
      name = 'string',
      loadArguments = {
        'attack',
      },
    },
    {
      name = 'number',
      loadArguments = {
        1,
      },
    },
    {
      name = 'bool',
      loadArguments = {
        false,
      },
    },
    {
      name = 'entity/dealdamage',
    },
    {
      name = 'number',
      loadArguments = {
        20,
      },
    },
  },
  links = {
    {
      1,
      1,
      6,
      1,
    },
    {
      1,
      2,
      9,
      1,
    },
    {
      1,
      3,
      9,
      2,
    },
    {
      1,
      3,
      8,
      1,
    },
    {
      1,
      3,
      10,
      2,
    },
    {
      2,
      1,
      1,
      1,
    },
    {
      3,
      1,
      1,
      2,
    },
    {
      4,
      1,
      1,
      3,
    },
    {
      5,
      1,
      1,
      4,
    },
    {
      7,
      1,
      14,
      4,
    },
    {
      8,
      1,
      14,
      2,
    },
    {
      8,
      1,
      9,
      3,
    },
    {
      9,
      1,
      10,
      1,
    },
    {
      10,
      1,
      14,
      1,
    },
    {
      11,
      1,
      10,
      3,
    },
    {
      12,
      1,
      10,
      4,
    },
    {
      13,
      1,
      10,
      5,
    },
    {
      15,
      1,
      14,
      3,
    },
  },
}