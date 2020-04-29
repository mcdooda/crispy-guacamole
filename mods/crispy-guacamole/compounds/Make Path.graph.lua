return {
  links = {
    {
      3,
      1,
      6,
      1,
    },
    {
      4,
      1,
      3,
      1,
    },
    {
      4,
      2,
      3,
      2,
    },
    {
      5,
      1,
      3,
      3,
    },
  },
  nodeType = 'script',
  nodes = {
    {
      loadArguments = {
        false,
      },
      name = 'boolean',
    },
    {
      loadArguments = {
        false,
      },
      name = 'boolean',
    },
    {
      name = 'map/settiletemplate',
    },
    {
      loadArguments = {
        {
          'IMPULSE',
          'In',
        },
        {
          3,
          'Tile',
        },
      },
      name = 'script/scriptinput',
    },
    {
      loadArguments = {
        'grass_light',
      },
      name = 'string',
    },
    {
      loadArguments = {
        {
          'IMPULSE',
          'Out',
        },
      },
      name = 'script/scriptoutput',
    },
  },
}