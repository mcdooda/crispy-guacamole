return {
  links = {
    {
      1,
      1,
      3,
      1,
    },
    {
      1,
      2,
      3,
      2,
    },
    {
      2,
      1,
      5,
      2,
    },
    {
      3,
      1,
      9,
      1,
    },
    {
      3,
      2,
      4,
      1,
    },
    {
      3,
      3,
      4,
      2,
    },
    {
      3,
      4,
      6,
      1,
    },
    {
      3,
      4,
      2,
      1,
    },
    {
      5,
      1,
      4,
      3,
    },
    {
      6,
      1,
      5,
      1,
    },
    {
      7,
      1,
      2,
      2,
    },
    {
      8,
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
          'IMPULSE',
          'In',
        },
        {
          3397857525273543980,
          'Entity',
        },
      },
      name = 'script/scriptinput',
    },
    {
      name = 'math/greaterthan',
    },
    {
      name = 'entity/headingchanged',
    },
    {
      name = 'entity/flipspritex',
    },
    {
      name = 'math/or',
    },
    {
      name = 'math/lessthan',
    },
    {
      loadArguments = {
        '5 * pi / 4',
      },
      name = 'math/formula',
    },
    {
      loadArguments = {
        'pi / 4',
      },
      name = 'math/formula',
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
  },
}