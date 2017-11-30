script {
  nodes = {
    {
      name = 'components/attackcomponent',
    },
    {
      name = 'number',
      loadArguments = {
        5,
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
        true,
      },
    },
    {
      name = 'number',
      loadArguments = {
        0,
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
      name = 'entity/lookatentity',
    },
    {
      name = 'entity/getattacktarget',
    },
    {
      name = 'entity/playanimation',
    },
    {
      name = 'string',
      loadArguments = {
        'shoot',
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
      name = 'entity/fireprojectile',
    },
    {
      name = 'string',
      loadArguments = {
        'arrow',
      },
    },
    {
      name = 'string',
      loadArguments = {
        'crossbow',
      },
    },
  },
  links = {
    {
      1,
      1,
      7,
      1,
    },
    {
      1,
      2,
      8,
      1,
    },
    {
      1,
      3,
      8,
      2,
    },
    {
      1,
      3,
      9,
      1,
    },
    {
      1,
      3,
      10,
      2,
    },
    {
      1,
      3,
      14,
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
      6,
      1,
      14,
      6,
    },
    {
      8,
      1,
      10,
      1,
    },
    {
      9,
      1,
      8,
      3,
    },
    {
      9,
      1,
      14,
      3,
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
      4,
    },
    {
      16,
      1,
      14,
      5,
    },
  },
}