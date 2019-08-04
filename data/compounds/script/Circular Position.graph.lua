return {
  links = {
    {
      1,
      1,
      10,
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
      2,
      12,
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
      12,
      2,
    },
    {
      1,
      4,
      7,
      2,
    },
    {
      5,
      1,
      11,
      1,
    },
    {
      6,
      1,
      11,
      2,
    },
    {
      7,
      1,
      11,
      3,
    },
    {
      8,
      1,
      5,
      2,
    },
    {
      10,
      1,
      5,
      1,
    },
    {
      10,
      2,
      6,
      1,
    },
    {
      10,
      3,
      7,
      1,
    },
    {
      11,
      1,
      9,
      1,
    },
    {
      12,
      1,
      6,
      2,
    },
  },
  nodeType = 'script',
  nodes = {
    {
      loadArguments = {
        {
          1409773913755924264,
          'Vector3',
        },
        {
          3,
          'd',
        },
        {
          3,
          'a',
        },
        {
          3,
          'Operand 2',
        },
        {
          'ANY',
          'New Pin',
        },
        {
          'ANY',
          'New Pin',
        },
      },
      name = 'script/scriptinput',
    },
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
      loadArguments = {
        'move',
      },
      name = 'string',
    },
    {
      name = 'math/add',
    },
    {
      name = 'math/add',
    },
    {
      name = 'math/add',
    },
    {
      loadArguments = {
        'd * cos(a)',
      },
      name = 'math/formula',
    },
    {
      loadArguments = {
        {
          1409773913755924264,
          'Vector3',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      name = 'math/vector3/vector3split',
    },
    {
      name = 'vector3',
    },
    {
      loadArguments = {
        'd * sin(a)',
      },
      name = 'math/formula',
    },
  },
}